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
	struct semid_ds *buffer;
	ushort array;
}arg;

void setsem(int semid,int semnum)
{
	arg.val=1;
	if(semctl(semid,semnum,SETVAL,arg)<0)
		printf("Error in setsem");
}

void wait(int semid,int semnum)
{
	struct sembuf b;

	b.sem_num=semnum;
	b.sem_op=-1;
	b.sem_flg=0;

	if(semop(semid,&b,1)<0)
		printf("Error in semop from wait\n");
}

void sig(int semid,int semnum)
{
	struct sembuf b;

	b.sem_num=semnum;
	b.sem_op=+1;
	b.sem_flg=0;

	if(semop(semid,&b,1)<0)
		printf("Error in semop from signl\n");
}


int main(int argc, char const *argv[])
{
	int semid,fd,shmid;
	char ch;
	key_t key1=1100;
	key_t key2=2500;

	char* buffer;

	shmid=shmget(key2,sizeof(int),IPC_CREAT | 0666);
	int *readcnt=(int*)shmat(shmid,NULL,0);

	
	
	fd=open("file.txt",O_CREAT | O_RDONLY);

	semid=semget(key1,2,IPC_CREAT | 0666);


	wait(semid,MUTEX);

	*readcnt=*readcnt+1;
	printf("No of Readers Active : %d\n",*readcnt);

	if(*readcnt==1)
		wait(semid,WRT);

	sig(semid,MUTEX);

	printf("Critical Section Start\n");
	
	while(read(fd, buffer, 20)!=0)
		printf("%s",buffer);

	printf("\n");

	gets(&ch);
	
	wait(semid,MUTEX);

	*readcnt=*readcnt-1;

	sig(semid,MUTEX);
	
	printf("Critical Section End\n");

	if(*readcnt==0)
		sig(semid,WRT);
	return 0;
}	

