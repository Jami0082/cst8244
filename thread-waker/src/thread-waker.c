#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *sem;
int numWakeup;

int main(void) {
	int x;
	sem=sem_open("/threadWaker",  0);
	numWakeup=0;
	printf("Waker Pid: %d\n", getpid()); /* prints Hello World!!! */
	scanf("%d", &numWakeup);

	do {
		printf("Enter the number of threads to wake(0 to exit):");
		scanf("%d", &numWakeup);
		for(x=0; x<numWakeup;x++)
			sem_post(sem);
		printf("\n");

	}while(numWakeup!=0);
	sem_close(sem);
	sem_destroy(sem);
	return EXIT_SUCCESS;
}
