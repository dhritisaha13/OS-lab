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

void send() {
	kill(p -> pid1, SIGUSR1);
}

void print() {
    	int i;
	
		for(i=0;i<100;i++)
	{
		
		if(p->msg[i]==0x04)
		break;
		putchar(p->msg[i]);
	}
	printf("\n");	
}

int main(int argc, char* argv[]) {
	int i=0;
	
	int shmID = shmget(atoi(argv[1]), sizeof(struct data), IPC_CREAT | 0666);
       
	p = (struct data*)shmat(shmID, NULL, 0);
	p -> pid2 = getpid();
	key_t g;
	g=atoi(argv[2]);
	p -> pid1 = g;
	
	signal(SIGUSR1, print);
	while(1) {
		p -> flag[2] = 1;
                p -> turn = 1;
                while(p -> flag[1] == 1 && p -> turn == 1);
                 
                
                p -> a = 0;
               printf("Chat 2:");
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
              
            
		send();
                p -> flag[2] = 0;
	}
	shmdt(p);
//	shmctl(shmID,IPC_RMID,NULL);
  //     printf("Shared memory removed successfully...");
	    
	return 0;
}
