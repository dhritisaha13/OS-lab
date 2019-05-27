#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

struct memory{
	
        int m;
	int n;
        int array[5][5];       
};

struct memory* a;
struct memory* b;

int avg_matrix(int m,int n,int a[][n],int i,int j)
{
	int avg=0;
	if(i==0 && j==0)
	avg=(a[i][j]+a[i+1][j]+a[i][j+1]+a[i+1][j+1])/4;
	if(i==(m-1) && j==(n-1))
	avg=(a[i][j]+a[i-1][j]+a[i][j-1]+a[i-1][j-1])/4;
	if(i==(m-1) && j==0)
	avg=(a[i][j]+a[i-1][j]+a[i][j+1]+a[i-1][j+1])/4;
	if(i==0 && j==(n-1))
	avg=(a[i][j]+a[i+1][j-1]+a[i][j-1]+a[i+1][j])/4;
	if(i!=0 && i!=(m-1))
	{
		if(j==0)
		avg=(a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j+1]+a[i-1][j+1]+a[i+1][j+1])/6;
		if(j==(n-1))
		avg=(a[i][j]+a[i-1][j]+a[i+1][j]+a[i][j-1]+a[i-1][j-1]+a[i+1][j-1])/6;
	}
	if(j!=0 && j!=(n-1))
	{
		if(i==0)
		avg=(a[i][j]+a[i][j-1]+a[i][j+1]+a[i+1][j-1]+a[i+1][j+1]+a[i+1][j])/6;
		if(i==(m-1))
		avg=(a[i][j]+a[i-1][j]+a[i][j+1]+a[i][j-1]+a[i-1][j-1]+a[i-1][j+1])/6;
	}
	if(i!=0 && j!=0 && i!=(m-1) && j!=(n-1))
		avg=(a[i][j]+a[i][j-1]+a[i][j+1]+a[i-1][j]+a[i-1][j-1]+a[i-1][j+1]+a[i+1][j]+a[i+1][j-1]+a[i+1][j+1])/9;
	return avg;
}

void input(int x,int y)
{
	int i,j,temp;
	
	for(i=0;i<x;i++)
	{
		for(j=0;j<y;j++)
		{
			temp=0;
			printf("\nEnter the array elements:\n");
	
			scanf("%d",&temp);
			(a) -> array[i][j]=temp;
		}
	}
}

int main()
{
	int i,j,p,q,c,d;
	key_t k=2,k1=3;	
	int shmID = shmget(k, sizeof(struct memory), IPC_CREAT | 0666);
	a = (struct memory*)shmat(shmID, NULL, 0);
	printf("Enter the dimension of the matrix:\n");
	scanf("%d %d",&p,&q);
	a->m=p;
	a->n=q;
	int s[p][q];
	input(a->m,a->n);
	for(i=0;i<p;i++)
	{
		for(j=0;j<q;j++)
		{
			
			s[i][j]=(a) -> array[i][j];
			printf(" A[%d][% d] =%d\n" ,i,j,s[i][j]);
		}
	}
	
	int shmID1= shmget(k1, sizeof(struct memory), IPC_CREAT | 0666);
	b = (struct memory*)shmat(shmID1, NULL, 0);
	b->m=p;
	b->n=q;
	pid_t pid1=fork();
	if(pid1 ==0)
	{
	
	for(i=0;i<(b->m*b->n);i++)
	{
		pid_t pid=fork();
		if(pid>0)
		{
			wait(NULL);
		}
		else if(pid==0)
		{
			c=i/q;
			d=i%q;
			b->array[c][d]=0;
			b->array[c][d]=avg_matrix(a->m,a->n,s,c,d);
		
		}
		else
			return -1;
	}
	}
	else if(pid1>0)
	{
	wait(NULL);

	
	  for(i=0;i<b->m;i++)
	   {
		for(j=0;j<b->n;j++)
		{
			
			printf("B[%d][%d] =%d \n ",i,j,(b) -> array[i][j]);
		}
	   }
	}
	else
	printf("fork1 failed");

	shmdt((void *)a);

	shmdt((void *)b);

	return 0;

}
		
			
		
			
	
		

