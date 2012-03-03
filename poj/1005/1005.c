#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int x, y;
	int i;
	unsigned int count;
	float pi = 3.14159;

	struct cart {
		int index;
		float x;
		float y;
		int year;
		struct cart *next;
		struct cart *prev;
	};


	struct cart *head;
	struct cart *cur;
	struct cart *tmp;

	static struct cart initial;
	initial.next = &initial;
	initial.prev = &initial;

	cur = head = &initial;

	scanf("%d", &i);
	if(i<=0)
		return -1;
	count = i;

	i = 1;
	while(i<=count) {
		//allocate a new struct
		tmp = malloc(sizeof(struct cart));
		memset(tmp, 0, sizeof(struct cart));

		//set the struct values
		scanf("%f %f", &tmp->x, &tmp->y);
		tmp->index = i;
//		tmp->year = x>y?((pi*tmp->x*tmp->x)/2/50.0 + 1):((pi*tmp->x*tmp->x)/2/50.0 + 1);
		tmp->year = (int)(pi*(tmp->x*tmp->x + tmp->y*tmp->y)/2/50.0) + 1;

		//move pointer cur to the new struct
		cur->next = tmp;
		tmp->prev = cur;
		tmp->next = NULL;
		cur = tmp;

		i++;
	}

	//move pointer cur to the list head;
	cur = head->next;
#if 0
	printf("cur 's p = %p\n", cur);	
	printf("head 's p = %p\n", head);	
	printf("initial 's p = %p\n", &initial);	
	printf("head->next 's p = %p\n", head->next);
	printf("cur->x=%f, y=%f\n", cur->x, cur->y);
#endif
	do {	
//		printf("%d %f %f\n", cur->index, cur->x, cur->y);
		printf("Property %d: This property will begin eroding in year %d.\n", cur->index, cur->year);
		cur = cur->next;
	} while(cur != NULL);

	printf("END OF OUTPUT.\n");
	return 0;		
}

