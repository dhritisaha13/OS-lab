#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>


// First part

int main()
{
 int fd,s;
 fd=open("foo1.txt", O_RDONLY | O_CREAT);
 char *f = (char *) calloc(100, sizeof(char)); 
 
 pid_t pid=fork();
 if(pid>0)
  {
     printf("It is the parent process");
     s = read(fd, f, 20);
     f[s] = '\0'; 
     printf("The bytes are: % s\n", f);
     int close(int fd);
  }
 else if(pid==0)
  {
    printf("It is the child process");
     s = read(fd, f, 20);
     f[s] = '\0'; 
     printf("The bytes are: % s\n", f);
   }
else
  
    printf(" Fork is not possible");
    

return 0;
}

// Second part
/*
int main()
{
 int fd,s;
 fd=open("foo1.txt", O_WRONLY | O_CREAT);
 
 
 pid_t pid=fork();
 if(pid>0)
  {
     printf("It is the parent process\n");
     s = write(fd, "Operating System Lab\n", strlen("Operating System Lab\n"));
     close(fd);
  }
 else if(pid==0)
  {
    printf("It is the child process\n");
     s = write(fd, "Second Half\n", strlen("Second Half\n"));
   }
else
  
    printf(" Fork is not possible");

 return 0;
}*/
    



