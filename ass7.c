#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MUTEX 0
#define WRT 1

union semun{
	int val;
	struct semid_ds *buf;
	ushort array;
}arg;

void setsem(int semid,int semnum)
{
	arg.val=1;
	if(semctl(semid,semnum,SETVAL,arg)<0)
		printf("Error in setsem");
}

int main(int argc, char const *argv[])
{
	char ch;
	int semid,shmid;
	key_t key1=1100;
	key_t key2=2500;
	
	semid=semget(key1,2,IPC_CREAT | 0666);
	printf("Semaphore ID: %d\n",semid);

	shmid=shmget(key2,sizeof(int),IPC_CREAT | 0666);
	int *readcnt=(int*)shmat(shmid,NULL,0);

	*readcnt=0;

	setsem(semid,WRT);
	setsem(semid,MUTEX);

	return 0;
}

