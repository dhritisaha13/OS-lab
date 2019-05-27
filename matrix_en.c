#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>


int m,n;


struct mem
{
	int b[10][10];
};

struct mem *sp;

int avg(int a[m][n],int i,int j,int m,int n)
{
	int sum=0,res;
	if(i==0 && j==0)
	{
		sum+=a[i][j]+a[i][j+1]+a[i+1][j]+a[i+1][j+1];
		res=(int)(sum/4);
	}
	else if(i==m-1 && j==n-1)
	{
		sum+=a[i][j]+a[i-1][j]+a[i-1][j-1]+a[i][j-1];
		res=(int)(sum/4);
	}
	else if(i==0 && j==n-1)
	{
		sum+=a[i][j]+a[i+1][j]+a[i+1][j-1]+a[i][j-1];
		res=(int)(sum/4);
	}
	else if(i==m-1 && j==0)
	{
		sum+=a[i][j]+a[i-1][j]+a[i-1][j+1]+a[i][j+1];
		res=(int)(sum/4);
	}
	else if(i==0 && j>0 && j<n-1)
	{
		sum+=a[i][j]+a[i][j-1]+a[i][j+1]+a[i+1][j-1]+a[i+1][j]+a[i+1][j+1];
		res=(int)(sum/6);
	}
	else if(i==m-1 && j>0 && j<n-1)
	{
		sum+=a[i][j]+a[i][j-1]+a[i][j+1]+a[i-1][j-1]+a[i-1][j]+a[i-1][j+1];
		res=(int)(sum/6);
	}
	else if(j==0 && i>0 && i<m-1)
	{
		sum+=a[i][j]+a[i-1][j]+a[i+1][j]+a[i-1][j+1]+a[i][j+1]+a[i+1][j+1];
		res=(int)(sum/6);
	}
	else if(j==n-1 && i>0 && i<m-1)
	{
		sum+=a[i][j]+a[i-1][j]+a[i+1][j]+a[i-1][j-1]+a[i][j-1]+a[i+1][j-1];
		res=(int)(sum/6);
	}
	else 
	{
		sum+=a[i][j]+a[i][j-1]+a[i][j+1]+a[i-1][j-1]+a[i-1][j]+a[i-1][j+1]+a[i+1][j-1]+a[i+1][j]+a[i+1][j+1];
		res=(int)(sum/9);
	}
	return res;
	
}

int main()
{
	key_t key=123456;
	int d,f=0;
	printf("Enter the no of rows and columns:");
	scanf("%d%d",&m,&n);
	printf("Enter the value of d:");
	scanf("%d",&d);
	int i,j,k,l, shmid,pid,a[m][n];
	printf("Enter the %d elements of the matrix:\n",(m*n));
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			scanf("%d",&a[i][j]);
		}
	}

	printf("\nINPUT MATRIX");
	for(i=0;i<m;i++)
	{
		printf("\n");
		for(j=0;j<n;j++)
		{
			printf("%d  ",a[i][j]);
		}
	}
	printf("\n");

	shmid = shmget(key,sizeof(struct mem),IPC_CREAT|0666);
	sp = (struct mem*)shmat(shmid,NULL,0);
	while(1){
		int f=0;

		for(i=0;i<m;i++)
		{
			for(j=0;j<n;j++)
			{
				for(k=0;k<m;k++)
				{
					for(l=0;l<n;l++)
					{	
						if(i!=k && j!=l)
							{
								if(a[i][j]>=a[k][l])
									{
										if(a[i][j]-a[k][l]>d)
											{
												f=1;
												break;
											}
									}

								else
									{
										if(a[k][l]-a[i][j]>d)
											{
												f=1;
												break;
											}

									}
						
							}
					
					 }
			
				 }
		
			 }
	
		  }

			


 if(f==1)
 {

		for(i=0;i<m;i++)
		{
			for(j=0;j<n;j++)
		{
			pid=fork();
			if(pid==0)
			{
				sp->b[i][j]=avg(a,i,j,m,n);
				exit(1);
			}
			else
			{
				wait(NULL);
			}
		}

		}


		for(i=0;i<m;i++)
		{
			for(j=0;j<n;j++)
		{
		
				a[i][j]=sp->b[i][j];
				
		}
		}	



	printf("\nAVERAGE MATRIX:");
	for(i=0;i<m;i++)
	{
		printf("\n");
		for(j=0;j<n;j++)
		{
			printf("%d  ",sp->b[i][j]);
		}
	}
	printf("\n");

 }
else
break;
}
	shmdt((void *)sp);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}


