#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

char **shell_split_line(char*) ;


int mysh_cd(char **args);
int mysh_exit(char **args);


char *builtin_str[] = {"cd","exit"};

int (*builtin_func[]) (char **) = {&mysh_cd,&mysh_exit};

int mysh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}


int mysh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "mysh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mysh");
    }
  }
  return 1;
}

int mysh_exit(char **args)
{
  printf("EXITING\n\n");
  return 0;
}

char *shell_read_line()
{
  char *cmd = NULL;
  int bufsize = 0;
  getline( &cmd, &bufsize, stdin );
  return cmd;
}

char **shell_split_line(char *cmd)
{
  int pos= 0 ;
  char **tokens = malloc(100 * sizeof(char*));
  char *token;
 
  token = strtok(cmd, " \n\r");
 
  while(token != NULL)
  {
    tokens[pos]=token;
    token = strtok(NULL, " \n\r");
    pos++;
  }
 
  tokens[pos]= NULL;
  return tokens;
}

int check_pipes(char *cmd)
{
  char *ops;
  char cpy[100];
  strcpy(cpy,cmd);
  ops = strtok(cpy, "|");
  if ( strcmp(ops,cmd)==0)
  {return 0;}
  else
  {return 1;}
}

int check_and(char * cmd)
{
  char *ops;
  char cpy[100];
  strcpy(cpy,cmd);
  ops = strtok(cpy, "&&");
  if ( strcmp(ops,cmd)==0)
  {return 0;}
  else
  {return 1;}
}

int check_or(char * cmd)
{
  char *ops;
  char cpy[100];
  strcpy(cpy,cmd);
  ops = strtok(cpy, "||");
  if ( strcmp(ops,cmd)==0)
  {return 0;}
  else
  {return 1;}
}
int check_semi(char * cmd)
{
  char *ops;
  char cpy[100];
  strcpy(cpy,cmd);
  ops = strtok(cpy, ";");
  if ( strcmp(ops,cmd)==0)
  {return 0;}
  else
  {return 1;}
}
//\n\r
int pipe_split_line(char *cmd)
{
  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,"|");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,"|");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];
  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);

  int pipefiledesc[2];

  pid_t child1,child2;

       pipe(pipefiledesc);

       child1 = fork();
       if (child1 == 0)
       {
          dup2(pipefiledesc[1], STDOUT_FILENO);
          close(pipefiledesc[0]);
          execvp(args1[0], args1);
          perror("Could not execute the first part");
          return 1;
       }

       child2 = fork();
       if (child2 == 0)
        {
          dup2(pipefiledesc[0], STDIN_FILENO);
          close(pipefiledesc[1]);
          execvp(args2[0], args2);
          perror("Could not execute the second part");
          return 1;
       }

       close(pipefiledesc[0]);
       close(pipefiledesc[1]);
       wait(NULL);
       wait(NULL);
       return 1;


}

int and_split_line1(char * cmd)
{
  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,"&&");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,"&&");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];
  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
 
       if ((child1 = fork())==0)
       {
          if(execvp(args1[0], args1)==-1);
          {
            perror("Could not execute the first part");
            return 0;
          }
       }
        else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
        if(child1<0)
         printf("Error\n");

       return 1;
}

int and_split_line2(char * cmd)
{

  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,"&&");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,"&&");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];

  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
  child2 = fork();
       if (child2 == 0)
       {
          if(execvp(args2[0], args2)==-1);
          {
            perror("Could not execute the second part");
            return 0;
          }
       }
       
	else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
        if(child1<0)
         printf("Error\n");
       return 1;
 
}
int or_split_line1(char * cmd)
{

  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,"||");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,"||");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];

  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
 
       if ((child1 = fork())==0)
       {
          if(execvp(args1[0], args1)==-1);
          {
            perror("Could not execute the first part");
            return 0;
          }
       }
	if(child1<0)
         printf("Error\n");
  	else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
        

       return 1;
}

int or_split_line2(char * cmd)
{
  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,"||");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,"||");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];

  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
  child2 = fork();
       if (child2 == 0)
       {
          if(execvp(args2[0], args2)==-1);
          {
            perror("Could not execute the second part");
            return 0;
          }
       }
 
	if(child1<0)
         printf("Error\n");   
	else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
          
  return 1;
 
}

int semi_split_line1(char * cmd)
{
  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,";");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,";");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];
  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
 
       if ((child1 = fork())==0)
       {
          if(execvp(args1[0], args1)==-1);
          {
            perror("Could not execute the first part");
            return 0;
          }
       }
        else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
        if(child1<0)
         printf("Error\n");

       return 1;
}

int semi_split_line2(char * cmd)
{

  char *ops;
  char *toks;
  char *parsedcmds[2];
  int pos=0;
  toks = strtok(cmd,";");
  do
         {
    parsedcmds[pos] = toks;
    toks = strtok(NULL,";");
    pos++;

  }while(toks != NULL);
  char *cmd1 = parsedcmds[0];
  char *cmd2 = parsedcmds[1];

  char **args1 = shell_split_line(cmd1);
  char **args2 = shell_split_line(cmd2);
  pid_t child1,child2;
  int count=1;
  child2 = fork();
       if (child2 == 0)
       {
          if(execvp(args2[0], args2)==-1);
          {
            perror("Could not execute the second part");
            return 0;
          }
       }
       
	else
        {
        
          wait(NULL);
          wait(NULL);
          return 1;
        }
        if(child1<0)
         printf("Error\n");
       return 1;
 
}


int shell_launch(char **args)
{
 
  if(args[0] == NULL)

  {
                printf("Empty command entered...\n");
   		 return 1;
  }
  int pid ;
  pid = fork();
  if (pid ==0)
  {
   
    if(execvp(args[0],args)==-1)
                    {
                      perror("Couldn't execute the given command");
                    }
    exit(0);
  }
  else if(pid > 0)
  {
   
    wait(NULL);
    return 1;
  }
  return 1;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < mysh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}

void shell()
{
  char *cmd ;
  char **args ;
  int status=1;
  do{
   
    printf("@");
    cmd = shell_read_line();
    printf("The Command is:%s\n",cmd);
   
	

      if(check_pipes(cmd)==0 && check_and(cmd)==0 && check_or(cmd)==0 && check_semi(cmd)==0)
      {
        args = shell_split_line(cmd);
        status = shell_execute(args);
       
      }
      if(check_and(cmd)==1)
      {
 
        status=and_split_line1(cmd);
        if(status==1)
        {
  
          status=and_split_line2(cmd);
        }
	
      }
/*	if(check_or(cmd)==1)
      {
 
        status=or_split_line1(cmd);

        if(status==0)
        {
  
          status=or_split_line2(cmd);
        }
      }*/
	if(check_semi(cmd)==1)
      {
 
        status=semi_split_line1(cmd);
 
        if(status==1)
        {
  
          status=semi_split_line2(cmd);
        }
      }	

      if(check_pipes(cmd)==1)
      {
   
        status = pipe_split_line(cmd);
      }
  
    free(cmd);
    free(args);
  }while(status);

}

int main( int argc, char **argv)
{
  printf("--------------Welcome------------\n");
  shell();
}






