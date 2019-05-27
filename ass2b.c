#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>


int main()
{
	int fd;
	fd=open("foo.txt",O_RDONLY);
	char arr[20];
	read(fd,arr,15);
	printf("%s\n",arr);
	return 0;
}

