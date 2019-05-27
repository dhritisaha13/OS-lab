#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int pfd[2];
	char c[2]={'.','/'};
	pid_t pid1,pid2;
	pid1=fork();
	
	if(pid1==0)
	{
		pipe(pfd);
		pid2=fork();
		if(pid2>0)
		{
			close(pfd[0]);
			dup2(pfd[1],1);
			close(pfd[1]);
			char *c1="write";
			char *c2[]={strcat(c,c1),NULL};
			execvp(c2[0],c2);
		}
		if(pid2==0)
		{
		//	wait(NULL);
			close(pfd[1]);
			dup2(pfd[0],0);
			close(pfd[0]);
			char *c3="wc";
			char *c4[]={strcat(c,c3),argv[1],NULL};
			execvp(c4[0],c4);
			
		}
	}
	else
	wait(NULL);
return 0;
}



