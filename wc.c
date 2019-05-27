#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char** argv)
{
	
	int fp,i=0;
	char s[100];
	char ch;
	int lc,wc,cc;
	lc=0;
	wc=0;
	cc=0;
    
   fp = 0;

	int r=read(fp, s, 100);
	while(s[i])
	{
		if(s[i]==' ' ||s[i]=='\n')
			wc++;
		if(s[i]=='\n')
			lc++;
		if(s[i]!='\n' ||s[i]!=' ')
			cc++;
		
		i++;
	}
	for(i=0;i<argc;i++)
	{
		if(strcmp(argv[i],"-l")==0)
		printf("lines= %d",lc);
		if(strcmp(argv[i],"-w")==0)
		printf("words= %d",wc);
		if(strcmp(argv[i],"-c")==0)
		printf("characters= %d",cc);
	}
	close(fp);
return(0);
}


