
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <semaphore.h>

#define NUMBERSFILE "numbers.txt"
#define BUFFERLEN 256

// global variables
sem_t sem;
int total_sum = 0;

// recursively calculate factorial
int factorialCalculator (int num) {
	if (num == 0) {
		return 1;
	} else {
		return num * factorialCalculator(num-1);
	}
}

void *factorial (void* arg) {
	int num = (intptr_t) arg;
	int factorial = 0;

	factorial =	factorialCalculator(num);
	printf("Factorial: %d\n", factorial);

	// critical section
	sem_wait(&sem);
		total_sum += factorial;
		printf("Total sum: %d\n", total_sum);
	sem_post(&sem);

	return NULL;
}

int main (void) {
	// variables
	int num, PID;
	pthread_t threads[5];
	FILE *file;

	sem_init(&sem, 0, 1); // initialise semaphore

	file = fopen(NUMBERSFILE, "w");

	printf("Please input 5 numbers: \n");
	for (int i = 0; i < 5; i++) {
		scanf("%d", &num); // get each input number
		fprintf(file, "%d\n", num);
	}
	fclose(file);

	PID = fork(); // create child process
	if (PID == 0) {
		file = fopen(NUMBERSFILE, "r");

		// create 5 threads, sending 1 number for each
		for (int i = 0; i < 5; i++) {
			fscanf(file, "%d", &num);
			pthread_create(&threads[i], NULL, factorial, (void *)(intptr_t)num);
		}

		// join all threads once they are done execution
		for (int i = 0; i < 5; i++) {
			pthread_join(threads[i], 0);
		}

		return 0; // signal that child process is done
	}

	wait(NULL); // waits for child process to close

	sem_destroy(&sem); // free semaphore
}