#include<stdio.h>
#include<stdlib.h>

int main()
{
	char f[100];
	int j;
	for(j=0;j<100;j++)
	{
		char t;
		t=getchar();
		if(t==0x04)
		break;
		f[j]=t;
	}
	printf("%s\n",f);
	
	return -1;
}	


