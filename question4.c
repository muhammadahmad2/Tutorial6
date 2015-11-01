#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBERS 10
#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE] = {0};
int in = 0;
int out = 0;

sem_t fill_count;
sem_t empty_count;

void *producer(void *num_array)
{
	int *numbers = (int*) num_array;
	for(int i=0; i<NUMBERS; i++){
		sem_wait(&empty_count);
		int index;
		sem_getvalue(&fill_count, &index);
		buffer[index] = numbers[i];
		printf("Produced %d\n", numbers[i]);
		sem_post(&fill_count);
		in = (in+1) % BUFFER_SIZE;
		sleep(rand() % 5);
	}
	return NULL;
}

void *consumer()
{
	for(int i=0; i < NUMBERS; i++){
		sem_wait(&fill_count);
		int index;
		sem_getvalue(&fill_count, &index);
		printf("Consumed %d\n", buffer[index]);
		buffer[index] = 0;
		sem_post(&empty_count);
		out = (out+1) % BUFFER_SIZE;
		sleep(rand() % 5);
	}
	return NULL;
}

int main(void)
{
	int numbers[NUMBERS];
	printf("Enter ten numbers: ");
	for(int i=0; i < NUMBERS; i++){
		scanf("%d", &numbers[i]);
	}
	
	pthread_t consumer_t;
	pthread_t producer_t;
	sem_init(&fill_count, 0, 0);
	sem_init(&empty_count, 0, BUFFER_SIZE);

	pthread_create(&producer_t, NULL, producer, (void*) &numbers);
	pthread_create(&consumer_t, NULL, consumer, NULL);

	pthread_join(consumer_t, NULL);
	pthread_join(producer_t, NULL);
	sem_destroy(&fill_count);
	sem_destroy(&empty_count);

	printf("Buffer: [ ");
	for(int i=0; i < BUFFER_SIZE; i++){
		printf("%d ", buffer[i]);
	}
	printf("]\n");
}
