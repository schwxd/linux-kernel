#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/platform_data/fake_battery.h>
#include <linux/mfd/88pm80x.h>
#include <linux/delay.h>

struct android_bat_info {
	struct android_bat_pdata *pdata;
	struct device *dev;
	struct power_supply psy_bat;
	struct delayed_work update_work;
	
	int voltage;
	int capacity;
	int online;
	int type;
	int health;
	int status;
	int temp;
	
};

/* struct android_bat_pdata { */
/* 	int (*get_capacity)(void); */
/* 	unsigned int curve[20][2]; */
/* }; */

static enum power_supply_property android_bat_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
};

#define NORMAL_POLL_TIME		(60)
#define POWER_CHANGE_POLL_TIME		(2)
#define DETECT_CHARGING_TIME		(10)

#define PM800_CHIP_ID			(0x00)
#define PM800_GPADC1_MEAS_EN    	(1 << 3)
#define PM800_INT_STATUS4       	(0x08)
#define PM800_GPADC_BD_EN       	(1 << 5)
#define PM800_GPADC1_MEAS_EN    	(1 << 3)

#define PM800_VBAT_MEAS_EN		(1 << 1)
#define PM800_INT_STATUS3		(0x07)
#define PM800_GPADC_BD_GP1_EN   	(1 << 6)
#define PM800_INT_ENA_3     		(0x0B)
#define PM800_INT_ENA_4			(0x0C)

#define BAT_BASE_TEMPERATURE   (-200)

static int ntc_resist_value[] = {
	/* -20C ~ -19C */
	68237, 64991, 61919, 59011, 56258, 53650, 51178, 48835, 46613, 44506,
	/* -10C ~ -1C */
	42506, 40600, 38791, 37073, 35442, 33892, 32420, 31020, 29689, 28423,
	/* 0C ~ 9C */
	27219, 26076, 24988, 23951, 22963, 22021, 21123, 20167, 19450, 18670,
	/* 10C ~ 19C */
	17926, 17214, 16534, 15886, 15266, 14674, 14108, 13566, 13049, 12554,
	/* 20C ~ 29C */
	12081, 11628, 11195, 10780, 10382, 10000,  9634,  9284,  8947,  8624,
	/* 30C ~ 39C */
	 8315,  8018,  7734,  7461,  7119,  6948,  6707,  6475,  6253,  6039,
	/* 40C ~ 49C */
	 5834,  5636,  5445,  5262,  5086,  4917,  4754,  4597,  4446,  4301,
	/* 50C ~ 59C */
	 4161,  4026,  3896,  3771,  3651,  3535,  3423,  3315,  3211,  3111,
	/* 60C ~ 69C */
	 3014,  2922,  2834,  2748,  2666,  2586,  2509,  2435,  2364,  2294,
	/* 70c ~ 79C */
	 2228,  2163,  2100,  2040,  1981,  1925,  1870,  1817,  1766,  1716,
	/* end */
	 0
};

static unsigned int poll_interval = NORMAL_POLL_TIME;
module_param(poll_interval, uint, 0644);
MODULE_PARM_DESC(poll_interval, "battery poll interval in seconds - " \
		"0 disables polling");

static int android_bat_get_property(struct power_supply *psy,
			enum power_supply_property psp,
			union power_supply_propval *val)
{
	struct android_bat_info *info =
		container_of(psy, struct android_bat_info, psy_bat);

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
		val->intval = info->status;
		break;
	case POWER_SUPPLY_PROP_CHARGE_TYPE:
		val->intval = info->type;
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = POWER_SUPPLY_HEALTH_GOOD;
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = 1;
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = info->online;
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = POWER_SUPPLY_TECHNOLOGY_LION;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = info->voltage;
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = info->capacity;
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = info->temp;
		break;
	default:
		return -EINVAL;
	}

	return 0;
	
}

static unsigned int test_curve[20][2] = {
	{3300, 0},
	{3415, 3},
	{3491, 6},
	{3533, 10}, 
	{3557, 15}, 
	{3585, 20}, 
	{3605, 25}, 
	{3620, 30}, 
	{3645, 40}, 
	{3680, 50}, 
	{3728, 60}, 
	{3754, 65}, 
	{3784, 70}, 
	{3859, 80}, 
	{3945, 90}, 
	{4100, 100}
};

static int calc_capacity(struct android_bat_info *info)
{
	int i;
	int voltage, capacity;
	int *curve;

	struct android_bat_pdata *pdata = info->pdata;

	i = sizeof(pdata->curve)/sizeof(unsigned int)/2;

	while (i) {
		if (info->voltage >= *pdata->curve[i])
			break;
		i--;
	}

	if (i == 20)
		capacity = 100;
	capacity = (info->voltage - pdata->curve[i][0]) / (pdata->curve[i+1][0] - pdata->curve[i][0]) + pdata->curve[i][1];

	return capacity;
}

static int get_pmic_voltage(void)
{
	int ret;
	int vol, vol_high, vol_low;

	ret = pm800_extern_read(PM80X_GPADC_PAGE, PM800_GPADC_MEAS_EN1);
	ret |= PM800_VBAT_MEAS_EN;
	ret = pm800_extern_write(PM80X_GPADC_PAGE, PM800_GPADC_MEAS_EN1, ret);

	vol_high = pm800_extern_read(PM80X_GPADC_PAGE, PM800_VBAT_MEAS1);
	vol_low = pm800_extern_read(PM80X_GPADC_PAGE, PM800_VBAT_MEAS2);

	vol = ((vol_high & 0xff) << 4) | (vol_low & 0x0f);
	vol = vol * 4 * 14 * 1000 / 10 / (2^12);

	return vol;
}

static int get_ntc_temperature(int resist)
{
	int i, temp;

	temp = BAT_BASE_TEMPERATURE;
	for (i = 0; 0 != ntc_resist_value[i]; i++) {

		if (resist >= ntc_resist_value[i]) {
			break;
		}
		temp += 10;
	}
	return temp;
}

static int read_ntc_value(void)
{
	int ret;
	int temp_h, temp_l, gpadc1_meas;
	int ntc_resist;
	int i;

	ret = pm800_extern_read(PM80X_GPADC_PAGE, PM800_STATUS_2);
	ret |= PM800_GPADC1_MEAS_EN;
	ret = pm800_extern_write(PM80X_GPADC_PAGE, PM800_STATUS_2, ret);

	ret = pm800_extern_read(PM80X_GPADC_PAGE, PM800_INT_STATUS4);
	ret &= ~PM800_GPADC_BD_EN;
	ret = pm800_extern_write(PM80X_GPADC_PAGE, PM800_INT_STATUS4, ret);

	msleep(10);

	gpadc1_meas = 0;
	for (i = 0; i<10; i++) {
		temp_h = pm800_extern_read(PM80X_GPADC_PAGE, PM800_GPADC1_MEAS1);
		temp_l = pm800_extern_read(PM80X_GPADC_PAGE, PM800_GPADC1_MEAS2);
		gpadc1_meas += temp_h << 4 | (temp_l & 0xf);
	}
	gpadc1_meas = gpadc1_meas / 10;
	ntc_resist = gpadc1_meas * 5;
	return ntc_resist;
}

static int get_pmic_temperature(void)
{
	int ret;
	int ntc_resist, temp;

	ntc_resist = read_ntc_value();

	temp = get_ntc_temperature(ntc_resist);

	ret = pm800_extern_read(PM80X_GPADC_PAGE, PM800_INT_STATUS4);
	ret |= PM800_GPADC_BD_EN;
	ret = pm800_extern_write(PM80X_GPADC_PAGE, PM800_INT_STATUS4, ret);

	ret = pm800_extern_read(PM80X_GPADC_PAGE, PM800_STATUS_2);
	ret &= ~PM800_GPADC1_MEAS_EN;
	ret = pm800_extern_write(PM80X_GPADC_PAGE, PM800_STATUS_2, ret);

	return temp;
}

static void update_work_func(struct work_struct *work)
{
	struct android_bat_info *info =
		container_of(work, struct android_bat_info, update_work.work);

	//get voltage from PMIC
	info->voltage = get_pmic_voltage();

	//get temperature from PMIC
	info->temp = get_pmic_temperature();

	if (info->pdata->get_capacity) {
		info->capacity = info->pdata->get_capacity();
	} else
		calc_capacity(info);

	power_supply_changed(&info->psy_bat);
	
	schedule_delayed_work(&info->update_work, poll_interval);
}

static int android_bat_probe(struct platform_device *pdev)
{
	struct android_bat_pdata *pdata = platform_get_drvdata(pdev);
	struct android_bat_info *info;
	int ret = 0;

	info = kzalloc(sizeof(struct android_bat_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->pdata = pdata;
	if (!info->pdata) {
		dev_err(&pdev->dev, "No platform data\n");
		ret = -EINVAL;
		goto err_pdata;
	}

	info->dev = &pdev->dev;
	platform_set_drvdata(pdev, info);
	
	info->psy_bat.name = "battery";
	info->psy_bat.type = POWER_SUPPLY_TYPE_BATTERY;
	info->psy_bat.properties = android_bat_props;
	info->psy_bat.num_properties = ARRAY_SIZE(android_bat_props);
//	info->psy_bat.set_property = android_bat_set_property;
	info->psy_bat.get_property = android_bat_get_property;
	
	ret = power_supply_register(info->dev, &info->psy_bat);
	if (ret) {
		dev_err(info->dev, "failed to register psy_bat\n");
		goto err_psy_reg;
	}

	INIT_DELAYED_WORK(&info->update_work, update_work_func);
	
	return 0;

err_psy_reg:
err_pdata:
	kfree(info);
	return ret;
}

#ifdef CONFIG_PM
static int android_bat_suspend(struct device *dev)
{
	return 0;
}
static int android_bat_resume(struct device *dev)
{
	return 0;
}

static struct dev_pm_ops android_bat_pm = {
	.suspend = android_bat_suspend,
	.resume = android_bat_resume,
};
#endif

static int android_bat_remove(struct platform_device *pdev)
{
	return 0;
}

static void android_bat_shutdown(struct platform_device *pdev)
{
}

static struct platform_driver android_bat_driver = {
	.driver = {
		.name = "android_bat",
		.owner = THIS_MODULE,
#ifdef CONFIG_PM
		.pm = &android_bat_pm,
#endif
	},
	.probe = android_bat_probe,
	.remove = android_bat_remove,
	.shutdown = android_bat_shutdown,
};

static int __init android_bat_init(void)
{
	return platform_driver_register(&android_bat_driver);
}

static void __exit android_bat_exit(void)
{
	platform_driver_unregister(&android_bat_driver);
}

module_init(android_bat_init);
module_exit(android_bat_exit);

MODULE_DESCRIPTION("Android fake battery driver");
MODULE_LICENSE("GPL");
