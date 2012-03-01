#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int x, y;
	int count, i;
	int pi = 3.14159;

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

	scanf("%d", &count);

	i = 1;
	while(i<=count) {
		tmp = malloc(sizeof(struct cart));
		memset(tmp, 0, sizeof(struct cart));

		scanf("%f %f", &tmp->x, &tmp->y);
		tmp->index = i;
		tmp->year = x<y?(pi*x*x/2/50.0):(pi*y*y/2/50.0);
		if (tmp->year < 0)
			tmp->year = 1;
		else
			tmp->year++;

		cur->next = tmp;
		tmp->prev = cur;
		tmp->next = NULL;
		cur = tmp;

		i++;
	}

//	tmp = count;
//	while(tmp--) {
//		printf("%d %f %f\n", head->index, head->x, head->y);
//	}
//
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

