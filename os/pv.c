#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 10

sem_t empty_sem;
sem_t full_sem;
pthread_mutex_t mutex;
int in = 0;
int out = 0;
int buf[M] = {0};

void print()
{
	int i;
	for (i=0; i<M; i++) {
		printf(" %d", buf[i]);
	}
	printf("\n");
}

void *producer(void *arg)
{
	for(;;) {
		sleep(1);
		sem_wait(&empty_sem);
		pthread_mutex_lock(&mutex);
		buf[in] = 1;
		printf("+produce one. buf:");
		print();
		in = (in + 1) % M;
		pthread_mutex_unlock(&mutex);
		sem_post(&full_sem);
	}
}

void *consumer(void *arg)
{
	for(;;) {
		sleep(2);
		sem_wait(&full_sem);
		pthread_mutex_lock(&mutex);
		buf[out] = 0;
		printf("-consume one. buf:");
		print();
		out = (out + 1) % M;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty_sem);
	}
}

int main(int argc, char *argv[])
{
	int ret;
	pthread_t p1, p2;
	int init1 = sem_init(&empty_sem, 0, M);
	int init2 = sem_init(&full_sem, 0, 0);

	if (init1 || init2) {
		printf("semaphore init failed.\n");
		exit(1);
	}

	int init3 = pthread_mutex_init(&mutex, NULL);
	if (init3) {
		printf("mutex init failed.\n");
		exit(1);
	}

	ret = pthread_create(&p1, NULL, &consumer, NULL);
	if (ret) {
		printf("pthread1 create failed.\n");
		exit(1);
	}

	ret = pthread_create(&p2, NULL, &producer, NULL);
	if (ret) {
		printf("pthread2 create failed.\n");
		exit(1);
	}

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	return 0;
}

