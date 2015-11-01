#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <semaphore.h>


int moving_sum[5] = {0,0,0,0,0};


struct info {
	int number;
	int index;
};

sem_t sem;

int count = 0;

void *factorial(void *arg)
{
    struct info *info;
	info = (struct info*) arg;



	int fact = 1;

	for (int i = 1; i <= info->number; i++)
		fact = fact * i;


    int a = 1;

    while(a==1)
    {
        // Create a critical section
        sem_wait(&sem);
        
        if (info->index == 0) {
        	moving_sum[info->index] = fact;
        	a=0;
        }
        if (moving_sum[(info->index) - 1] > 0) {
        	fact += moving_sum[(info->index) - 1];
        	moving_sum[info->index] += fact;
        	a = 0;
        }

        // End of the critical section
        sem_post(&sem);

        usleep(1);
    }

    return NULL;
}

int main (void) {
	pthread_t numThreads[5];
	struct info information[5];

	printf("Input 5 values:\n");
	for (int i = 0; i < 5; i++) {
		scanf("%d", &information[i].number);
		information[i].index = i;
	}

    sem_init(&sem, 0, 1);

    for (int i = 0; i < 5; i++) {
		pthread_create(&numThreads[i], NULL, factorial, (void *) &information[i]);
	}

	for (int i = 0; i < 5; i++) {
		pthread_join(numThreads[i], NULL);
	}

	printf("Moving sum:\n");
	for(int s = 0; s < 5; s++) {
    	printf("%d \n", moving_sum[s]);
	}

    sem_destroy(&sem);

    return 0;
}