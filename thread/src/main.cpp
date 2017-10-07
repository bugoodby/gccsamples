#include "main.h"
#include "EventHandleTest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main( int argc, char **argv )
{
	printf("--- main start ---\n");
	
	EventHandleTest *pInstance = new EventHandleTest;
	
	pInstance->start();
	
	delete pInstance;
	
#if 0
	int pid = getpid();
	printf("--- main[pid=%d] start ---\n", pid);

	pthread_t thread;
	pthread_create(&thread, NULL, &thread_main, NULL);
	
	pthread_join(thread, NULL);
	
#endif
	printf("--- main end ---\n");
	return 0;
}

