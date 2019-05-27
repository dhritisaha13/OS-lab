#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct memory
{
   int turn;
   int no;
}*p;  

int main()
{
   key_t key=1111;
   int shmid,k=1;

   shmid=shmget(key,100,IPC_CREAT|0666);
   if(shmid<0)
     {
       printf("shmget error\n");
       exit(1);
     }
   printf("Client has received a shared memory...\n");
  
   p =(struct memory *)shmat(shmid,NULL,0);
   if((int *) p == -1)
    {
      printf("shmat error\n");
      exit(1);
    }
   printf("client has attached the memory...\n");

   p->turn=1;
   while(1)
    {
      while(p->turn!=1);
        p->no=k;
        k++;
        p->turn=2;
        if(k==12)
          break;
    }
  
   shmdt((void *)p);
   shmctl(shmid,IPC_RMID,NULL);
}
