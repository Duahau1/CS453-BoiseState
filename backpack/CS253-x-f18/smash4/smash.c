#include <stdio.h>
#define _POSIX_SOURCE
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#define MAXLINE 4096
#define MAXARGS 2048
#include "history.h"

void printToken(char *a);
char* strncpy_safe(char *dst, const char *src, size_t len);
void successChange(char *b);
void myHandle(int hand);
void pipeHandler(char * args[]);
char ** argument_sep(char *line);
char ** chop_exe(char *line);
char target1[65];
char target2[65];
void fileIO(char *args[],char* inputFile, char*outputFile,int option);
bool level=false;
bool need =false;
bool attach =true;
int err=0;
int on, off;
 char in[65];
 char output[65];
int waits, status;

int main()
{


  
char **cmdA;
char **cmdB;

struct hist *history;
history=init_history();


char buff1[MAXLINE];
char buff2[MAXLINE];
char buff3[MAXLINE];

fprintf(stderr,"$");
//Print $
while (fgets(buff1, MAXLINE, stdin) != NULL) {
   signal(SIGINT,myHandle);
  if(strlen(buff1)==1){
fprintf(stderr,"$");  
}
else{
int count=strlen(buff1);
count--;
buff1[count]='\0'; //replace newline with NULL

strncpy(buff2,buff1,MAXLINE);
strncpy(buff3,buff1,MAXLINE);
char *token = strtok(buff2," ");


add_history(history,buff1);

if (strcmp("exit",token)==0) {
  clear_history(history);
  
  exit(0);
}

else if(strcmp("cd",token)==0){
char *ch=strtok(buff1," ");

  successChange(ch);  

}

else if (strcmp("history",token)==0){
print_history(history);
  
}

else {
int i=0;
int j=0;
bool flag =false;
attach=true;
err=0;

level=false;
int aux;
on=0;
off=0;
memset(in,0,65);
memset(output,0,65);
memset(target1,0,65);
memset(target2,0,65);
char *cmd_aux[MAXLINE];
int index1;

for(index1=0;index1<MAXLINE;index1++){
cmd_aux[index1]=NULL;
  
}

cmdA=argument_sep(buff1);
cmdB=chop_exe(buff3);

while(cmdA[j]!=NULL){
if((strcmp(cmdA[j],">")==0)||(strcmp(cmdA[j],"<")==0)){
  break;
}
 cmd_aux[j] =cmdA[j];
 j++;
}

while(cmdA[i]!=NULL){

  if(strcmp(cmdA[i],"<")==0){
aux=i+1;
flag =true;
attach=false;
if(cmdA[aux]==NULL||cmdA[aux+1]==NULL){
printf("Not enough input argument\n"); 
break;
}
cmd_aux[aux+3]=NULL;
if((cmdA[i+3]!=NULL) && (strcmp(cmdA[i+2],">")==0)){
level =true;
}

 
if(level==true){

  
  fileIO(cmd_aux,cmdA[i+1],cmdA[i+3],2);
  
	if(err==-1){
 
  clear_history(history);
  free(cmdA); 
    
  }
}
else if(level==false){
fileIO(cmd_aux,cmdA[i+1],cmdA[i+3],1);  
 
 
if(err==-1){
   
clear_history(history);
free(cmdA); 
    
  }
  
}

break;  
}
else if(strcmp(cmdA[i],">")==0){
flag =true;
attach=false;

if(cmdA[i+1]==NULL){
printf("Not enough input argument\n");
break;
}

fileIO(cmd_aux,NULL,cmdA[i+1],0);
break;
}  
if(strcmp(cmdA[i],"|")==0){
pipeHandler(cmdA);

flag=true;  
break;
}  
  
i++;  
}

cmd_aux[i]=NULL;

  if(on==1&& off==1){
    fileIO(cmdB,in,output,2);
    
  }
  else if(on==0 &&off==1){
  
    fileIO(cmdB,NULL,output,0);
    
  }
   else if(on==1 &&off==0){
  
     fileIO(cmdB,in,NULL,1);
   
  }
else if(on==0&&off==0){
 attach=false;
  
} 


if(attach==false&&flag==false){

pid_t node;
node=fork();

if(node==-1){
printf("Child process could not be created\n");
  
}
else if(node==0){
if (execvp(cmdA[0],cmdA)==-1){
	printf("Command not found\n");
	clear_history(history);
free(cmdA); 
free(cmdB);
exit(1);		
	
}
  
}
else if(node>0){
int x,exitStatus;
	x=wait(&exitStatus);
	printf("PID %5d exited, status = %d\n",x,exitStatus);
  
}
}
free(cmdA);
free(cmdB);
}  

fprintf(stderr,"$");
  
}
  
}

clear_history(history);

return 0;
}

void successChange(char *token){
token = strtok(NULL," ");
if (token==NULL){
chdir(getenv("HOME"));
printf("%s\n",getenv("HOME")); 
}
else{
int dir=chdir(token);
if(dir ==0){

char *dirch=getcwd(NULL,0);
printf("%s\n", dirch);  
free(dirch);
}
else{
printf("error: ");
printf(token);
printf(" does not exist\n");  
} 
}  
}


/*
 * To seperate all the arguments
 */

char ** argument_sep(char *input){
  
  char **cmd=malloc(sizeof(char *)*MAXLINE);
  
  int n;
  for(n=0; n< MAXLINE; n++){
  cmd[n]=NULL;
  }
  
  char *token;
  int index=0;
  
  token=strtok(input," ");
  
  
  while(token !=NULL){
  
  cmd[index]=token;
  
  index++;
  token = strtok(NULL, " ");
  
    
  }
  cmd[index]=NULL;
  

  return cmd;
}  

char ** chop_exe(char *input){
  
  char **cmd=malloc(sizeof(char *)*MAXLINE);
  
    int n;
  for(n=0; n< MAXLINE; n++){
  cmd[n]=NULL;
  }
  
  char *token;
  
  int index=0;
  
  
  token=strtok(input," ");
  
  
  while(token !=NULL){
 
  
  if(*token=='<'&&strlen(token)>1){
    int len=strlen(token)+1;
    strcpy(target1,token);
    strncpy(in,token+1,len);
   
    on=1;
    
  token= strtok(NULL," ");
  
    if(token==NULL){
  
    break;
  }
  }
  if(*token=='>'&&strlen(token)>1){
     int len=strlen(token)+1;
    
     strcpy(target2,token);
    
     strncpy(output,token+1,len);
   
  
    off=1;
    
  token= strtok(NULL," ");
    if(token==NULL){
  
    break;
  }
    } 
 
  
  cmd[index]=token;
  
  index++;
  token = strtok(NULL, " ");
  }
 
  cmd[index]=NULL;
  
 
  
  return cmd;
}  
/*
 *File I/O redirection
 * 
 * 
 */  

void fileIO(char *args[],char* inputFile, char*outputFile,int option){

 
	pid_t pid;
	int fileDescriptor; 

	if((pid=fork())==-1){
		printf("Child process could not be created\n");
	}
	if(pid==0){
		// Option 0: output redirection
		if (option == 0){
			// We open (create) the file truncating it at 0, for write only
			fileDescriptor = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0664); 
			
			// Replace the standard output with the appropriate file
			dup2(fileDescriptor, STDOUT_FILENO); 
			close(fileDescriptor);
		// Option 1: input and output redirection
		}else if (option == 1){
			// We open file for read only (it's STDIN)
			fileDescriptor = open(inputFile, O_RDONLY);  
			if(fileDescriptor<0){
			printf("Not found file\n");
			err=-1;
			exit(1);  
			}
			
			// We replace de standard input with the appropriate file
			dup2(fileDescriptor, STDIN_FILENO);
			close(fileDescriptor);
			 
		}
		else if(option ==2){
			fileDescriptor = open(inputFile, O_RDONLY);  
			if(fileDescriptor<0){
			printf("Not found file\n");
			err=-1;
			exit(1);  
			}
			// We replace de standard input with the appropriate file
			dup2(fileDescriptor, STDIN_FILENO);
			int fileDescriptor2;
			// Same as before for the output file
			
			fileDescriptor2 = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY,0664);
			dup2(fileDescriptor2, STDOUT_FILENO);
			close(fileDescriptor);
			close(fileDescriptor2);	
		  
		}
		
		execvp(args[0],args);
			printf("Not found command : %s \n",args[0]);
			exit(1);	 
	}
	
int x,exitStatus;
	x=wait(&exitStatus);
	printf("PID %5d exited, status = %d\n",x,exitStatus);
}
/**
* Method used to manage pipes.
*/ 
void pipeHandler(char * args[]){
	// File descriptors
	int filedes[2];  // 0 input of the file and 1 output of the file 
	int filedes2[2]; // 0 input of the file and 1 output of the file 
	int len  = 0;
	 setbuf(stdout,NULL);
	int num_cmds = 0;
	int move=0;
	int opt=-1;

	
	pid_t pid;
	
	
	char *maincmd[MAXLINE];
	int g;
	for (g=0;g<MAXLINE;g++){
	maincmd[MAXLINE]=NULL; 
	}
	int end = 0;
	
	int j=0;
	int pk;
	
	while (args[len] != NULL){
	  if(strcmp(args[len],"*.c")==0){
		char * ch= "smash.c";
		 args[len]=ch;
	  }
	  
	  if (strcmp(args[len],"|") == 0){
		
		  num_cmds++;
		}
		
		len++;
	  
	}
	num_cmds++;
	if(on==1&& off==1){
     
	    on=0;
	  off=0;
	  opt=2;
	  
	}
    else if(on==0 &&off==1){
   
      on=0;
      off=0;
      opt=0;
    }
   else if(on==1 &&off==0){
   
      on=0;
     off=0;
     opt=1;
     
  }
  else if(on==0&&off==0){
      opt=-1;
    
      attach=false;
  }
	
	//THe main loop to read in all the argument
	while(args[j]!=NULL && end!=1){
	pk=0;
	
	memset(maincmd,0,MAXLINE);
      
	while (strcmp(args[j],"|") != 0){
	
	  if((strcmp(args[j],target1)==0)||(strcmp(args[j],target2)==0 )){
	  
	  j++;
	 
	  if (args[j] == NULL){
				
			    end = 1;
			    break;
			}
	   
	 if(strcmp(args[j],"|")==0){
	    pk--;
	    
	  }
	   }
	
	  if (strcmp(args[j],"|") != 0 &&args[j]!=NULL){
	  maincmd[pk] = args[j];
	  j++;	
	}
	 
		  if (args[j] == NULL){
				
			    end = 1;
			    pk++;
				break;
			}
	      pk++;
		}
		
		maincmd[pk] = NULL;
		
		j++;	
	
	if (move % 2 != 0){
			if (pipe(filedes) < 0) {
		perror("Error creating pipe1");
		    exit(-1);
			  }; // for odd i
		}else{
			if (pipe(filedes2) < 0) {
		    perror("Error creating pipe2");
		    exit(-1);
		  } // for even i
		}
		
	
	pid=fork();
	if(pid==-1){			
	if (move != num_cmds - 1){
	if (move % 2 != 0){
	  close(filedes[1]); // for odd i
	}else{
	close(filedes2[1]); // for even i
	} 
	    }			
	printf("Child process could not be created\n");
	  exit(-1);
	  }
	if(pid==0){
	// If we are in the first command
			if (move == 0){
			
			  close(1);
			  
			dup2(filedes2[1],1);
			  if(opt==1 ||opt ==2){
	
		  filedes2[0] = open(in, O_RDONLY,0664);  
			if(filedes2<0){
			printf("Not found file\n");
			printf("%d The err\n",err);
			exit(1);  
			}
			
			// We replace de standard input with the appropriate file
			dup2(filedes2[0],0);
			close(filedes2[0]);
		  }
			 
			  
	}
	  else if(move==num_cmds-1){
	  
	    if (num_cmds % 2 != 0){ // for odd number of commands
					
		  close(0);
	
		  dup2(filedes[0],0);

	      if(opt==0|| opt==2){ 
		int fd;
		    fd= open(output, O_CREAT | O_TRUNC | O_WRONLY, 0664); 
			
			// Replace the standard output with the appropriate file
			dup2(fd, 1); 
			close(fd);
	  
	}
	      
	    }else{ 
	// for even number of commands
	 
	close(0);
	    dup2(filedes2[0],0);

	
	if(opt==0|| opt==2){
	filedes2[1] = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0664); 
			
			// Replace the standard output with the appropriate file
			dup2(filedes2[1], 1); 
			close(filedes2[1]); 
	}   
	    }
	      
	  }
	else{
	if (move % 2 != 0){
					dup2(filedes2[0],0); 
					dup2(filedes[1],1);
				}else{ // for even i
					dup2(filedes[0],0); 
					dup2(filedes2[1],1);					
				} 
	
	  
	}
	if (move == 0){
			close(filedes2[1]);
		}
		else if (move == num_cmds - 1){
			if (num_cmds % 2 != 0){					
				close(filedes[0]);
			}else{					
				close(filedes2[0]);
			}
		}else{
			if (move % 2 != 0){					
				close(filedes2[0]);
				close(filedes[1]);
			}else{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}
	if (execvp(maincmd[0],maincmd)==-1){
		if (move != num_cmds - 1){
	if (move % 2 != 0){
	  close(filedes[1]); // for odd i
	}else{
	close(filedes2[1]); // for even i
	} 
	    }			
		fprintf(stderr,"Not found command : %s \n",maincmd[0]);
		exit(1);
	  
	}
	  
	}// CLOSING DESCRIPTORS ON PARENT
		if (move == 0){
			close(filedes2[1]);
		}
		else if (move == num_cmds - 1){
			if (num_cmds % 2 != 0){					
				close(filedes[0]);
			}else{					
				close(filedes2[0]);
			}
		}else{
			if (move % 2 != 0){					
				close(filedes2[0]);
				close(filedes[1]);
			}else{					
				close(filedes[0]);
				close(filedes2[1]);
			}
		}
		
	
	 
	 int exitStatusA, x;
	x=wait(&exitStatusA);
	printf("PID %5d exited with %d\n",x,exitStatusA);

	move++;
	  
	}
	
}
/*TO handle the signal 1
 */
void myHandle(int hand){
  if(hand == SIGINT){
    fprintf(stderr,"\n");
	} 
}
/*
*To print out the token
*/
void printToken(char *token){
int i = 0;
while (token != NULL){
printf ("[%d] %s\n",i, token);
token = strtok (NULL, " ");
i++;
  
}
i=0;
}

