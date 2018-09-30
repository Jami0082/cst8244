#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

sig_atomic_t handlerCallCheck=0;

sig_atomic_t userHappened=0;
sig_atomic_t numFinished;
int numChildren;
int nThreads;

sig_atomic_t parent;
sem_t * sem;
pthread_t tid[2];

void sigint_handler(int sig);

void* threadMaker(void* arg)
{
	printf("Thread %d created\n", pthread_self() );
	while(1) {
		sem_wait(sem);
		printf("Thread %d unblocked\n", pthread_self());
		sleep(5);
	}
	return NULL;
}

int main(void)
{
	numChildren=0;
	numFinished=0;
	userHappened=0;
	parent=0;
	nThreads=0;

	pthread_attr_t attr;

	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART

	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("Enter the number of threads: ");
	scanf("%d", &nThreads);

	sem=sem_open("/threadWaker", O_CREAT, S_IWOTH, 0);

	for(int i = 0; i < nThreads; i++) {
		pthread_attr_init(&attr);
		pthread_create(NULL, &attr, &threadMaker, NULL);
		pthread_attr_destroy(&attr);
	}

	while( userHappened==0)
	{}
	printf("PID = %d: Children finished, parent exiting.\n", getpid());
	sem_close(sem);
	sem_destroy(sem);
	return EXIT_SUCCESS;
}

void sigint_handler(int sig) {
	userHappened=1;
}

