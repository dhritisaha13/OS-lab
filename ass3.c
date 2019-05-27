#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>

int main( int argc, char *argv[] )
{
int k;
pid_t pid;

for(k=1;k<=argc;k++)
{
  char *args[]={argv[k],NULL};
   pid=fork();
   if(pid>0)
    {
       wait(NULL);
    }
   else if(pid==0)
    {
        execvp(args[0],args);
        printf("error\n");
    }
   else 
    {
        printf("The system fails\n");
        return -1;
    }
}

return 0;
}
