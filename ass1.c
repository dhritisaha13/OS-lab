#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include<stdlib.h>


long int fact(int n)
{
    if (n >= 1)
        return n*fact(n-1);
    else
        return 1;
}


int main( int argc, char *argv[] )
{
	int j,n;
	pid_t pid;


	for(j=1;j<=argc;j++)
	{
   
     		n=atoi(argv[j]);
     		pid=fork();
		if(pid>0)
		{
 		  break;
		}
		else if(pid==0)
		{
   
		long int factorial=fact(n);
 		printf("the factorial of number %d is %ld\n",n,factorial);

		}
		else 
		{
		printf("The system fails\n");
		return -1;
		}
	}

return 0;
}

