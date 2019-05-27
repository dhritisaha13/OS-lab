#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

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
	int semid,fd;
	key_t key1=1100;
	
	
	char c;
	char* buffer;

	fd=open("file.txt",O_CREAT | O_WRONLY | O_APPEND);

	semid=semget(key1,2,IPC_CREAT | 0666);
	printf("Semid : %d\n",semid);

	wait(semid,WRT);


	printf("Critical Section Start\n");
	printf("Enter input to be written: ");
	scanf("%s",buffer);
	buffer[strlen(buffer)]='\n';
	write(fd,buffer,strlen(buffer));
	
	gets(&c);
	close(fd);

	gets(&c);

	sig(semid,WRT);
	printf("Critical Section End\n");
	return 0;
}
