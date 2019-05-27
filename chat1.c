#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>

struct data {
	char msg[100];
        int pid1, pid2;
        int flag[3];
	int turn;
        int a;
};

struct data* p;

void send() 
{
	kill(p -> pid2, SIGUSR1);
}

void print() 
{
    	int i;
	
		for(i=0;i<100;i++)
	{
		
		if(p->msg[i]==0x04)
		break;
		putchar(p->msg[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[]) 
{
	
	int i=0;
	int shmID = shmget(atoi(argv[1]), sizeof(struct data), IPC_CREAT | 0666);
        
	p = (struct data*)shmat(shmID, NULL, 0);
	p -> pid1 = getpid();
	printf("%d %d\n", p -> pid1, p -> pid2);
	signal(SIGUSR1, print);
	while(1) {
		p -> flag[1] = 1;
                p -> turn = 2;
                while(p -> flag[2] == 1 && p -> turn == 2);
                 
              
                p -> a = 0;
               printf("Chat 1:");
                for(i=0;i<100;i++)
               		p->msg[i]='\0';
                for(i=0;i<100;i++)
	{
		
		char temp;
		temp=getchar();
		if(temp==0x04)
		break;
		p->msg[i]=temp;
	}
            		
			p -> a += 1;		
               
		
		send();
                p -> flag[1] = 0;

	}
	shmdt(p);
//	shmctl(shmID,IPC_RMID,NULL);
  //     printf("Shared memory removed successfully...");
	
	return 0;
}
