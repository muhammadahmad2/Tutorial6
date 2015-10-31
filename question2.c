
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void) {
	int PID;
	int status;

	PID = fork();
	if (PID == 0) {
		sleep(1);
		printf("Child process\n");
		return 0;
	}

	wait(&status);
	printf("Child process return value: %d\n", status);
	printf("Parent process\n");
}