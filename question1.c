
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#define child1File "child1.txt"
#define child2File "child2.txt"

int main(void) {
	int PID1, PID2;
	FILE *file;

	file = fopen(child1File, "w");
	fprintf(file, "%s", "child1");
	fclose(file);

	file = fopen(child2File, "w");
	fprintf(file, "%s", "child2");
	fclose(file);

	PID1 = fork();
	if (PID1 == 0) {
		sleep(1);
		file = fopen(child1File, "r");
		char line[7];
		fgets(line, 7, file);
		printf("%s\n", line);
		fclose(file);
		return 0;
	}

	PID2 = fork();
	if (PID2 == 0) {
		sleep(1);
		file = fopen(child2File, "r");
		char line[7];
		fgets(line, 7, file);
		printf("%s\n", line);
		fclose(file);
		return 0;
	}

	wait(NULL);
}