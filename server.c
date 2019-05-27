#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

struct memory
{
   int turn;
   int no;
}*p;
void factorial(int value);


int main()
 {
      char ch;
      int shmid,i;
      key_t key=1111;
  
  
      shmid=shmget(key,100,IPC_CREAT|0666);
      if(shmid<0)
        {
           printf("shmget error\n");
           exit(1);
        }
   
      p=(struct memory *) shmat(shmid,NULL,0);
      if((int *)p == -1)
        {
           printf("shmat error\n");
           exit(1);
        }
       printf("Server has attached the shared memory...\n");
       p->turn=1;
       while(1)
          {
             while(p->turn!=2);
               factorial(p->no);
                p->turn=1;
              if(p->no==12)
                 break;
          }
       shmdt((void *)p);
       shmctl(shmid,IPC_RMID,NULL);
       printf("Shared memory removed successfully...");
}
  
void factorial(int n)
{
  int m;
  long long int t=1;
  for(m=1;m<=n;m++)
     t=t*m;
  printf("%lld\n",t);
}

