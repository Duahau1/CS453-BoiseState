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
void pipeHandler(char * args[]);
char ** argument_sep(char *line);
char ** chop_exe(char *line);
char target1[65];
char target2[65];
void fileIO(char *args[],char* inputFile, char*outputFile,int option);
void myHandle1(int a);
void myHandle2(int b);
bool level=false;
bool need =false;
bool attach =true;
int err=0;
int on, off;
 char in[65];
 char output[65];
int waits, status;
char buff1[MAXLINE];
char buff2[MAXLINE];
char buff3[MAXLINE];

int main()
{



char **cmdA;
char **cmdB;
//pid_t pid;
struct hist *history;
history=init_history();




fprintf(stderr,"$");
//Print $
while (fgets(buff1, MAXLINE, stdin) != NULL) {
  signal(SIGINT,myHandle1);
  if(strlen(buff1)==1){
   // signal(SIGINT,myHandle2);
    fprintf(stderr,"$");  
}
else{
int count=strlen(buff1);
count--;
buff1[count]='\0'; //replace newline with NULL

strncpy(buff2,buff1,MAXLINE);
strncpy(buff3,buff1,MAXLINE);
char *token = strtok(buff2," ");

//
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
//need=false;
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
}
cmd_aux[aux+3]=NULL;
if((cmdA[i+3]!=NULL) && (strcmp(cmdA[i+2],">")==0)){
level =true;
}

 
if(level==true){
  fileIO(cmd_aux,cmdA[i+1],cmdA[i+3],2);
  if(err==-1){
  printf("%d The err\n",err);
  clear_history(history);
  free(cmdA); 
    
  }
}
else if(level==false){
fileIO(cmd_aux,cmdA[i+1],cmdA[i+3],1);  
 printf("%d The err\n",err);
 
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
/*TO handle the signal 1
 */
void myHandle1(int hand){
  if(hand == SIGINT){
  
  if(strlen(buff1)==1){
      fprintf(stderr,"\n"); 
    }
    else{
    fprintf(stderr,"\n");
    }
   }
}
/*TO handle the signal 2
 */
void myHandle2(int hand){
  if(hand == SIGINT){
      printf("^C\n");
    fprintf(stderr,"$");
  }
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
 * 
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
  //cmd[index]=token;
  
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
  
  // memset(in,0,65);
  // memset(output,0,65);
    
  
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
			 printf("%d The err\n",err);
			exit(0);  
			}
			
			// We replace de standard input with the appropriate file
			dup2(fileDescriptor, STDIN_FILENO);
			close(fileDescriptor);
			// Same as before for the output file
					 
		}
		else if(option ==2){
			fileDescriptor = open(inputFile, O_RDONLY);  
			if(fileDescriptor<0){
			printf("Not found file\n");
			err=-1;
			exit(0);  
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
			printf("err\n");
			
				 
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
	int filedes2[2];
	int len  = 0;
	 setbuf(stdout,NULL);
	int num_cmds = 0;
	int opt=-1;
	int count=0;
	int hit=0;
	int track=0;
	char *command1[MAXLINE];
	char *command2[MAXLINE];
	char *command3[MAXLINE];
	pid_t pid;
	pid_t pid2;
	pid_t pid3;
	
	//int end = 0;
	
	// Variables used for the different loops
	//printf("The target1 %s\n",target1);
	//printf("The target1 %s\n",target2);
	
	while (args[len] != NULL){
	
	  if (strcmp(args[len],"|") == 0){
		
		  num_cmds++;
		}
		
		len++;
	  
	}
	num_cmds++;
	
	
	
	
	
	if(on==1&& off==1){
      //fileIO(args,in,output,2);
	    on=0;
	  off=0;
	  opt=2;
	  
	}
    else if(on==0 &&off==1){
    //printf("the out %s\n",output);
   // fileIO(args,NULL,output,0);
      on=0;
      off=0;
      opt=0;
    }
   else if(on==1 &&off==0){
    //fileIO(args,in,NULL,1);
      on=0;
     off=0;
     opt=1;
     
  }
  else if(on==0&&off==0){
      opt=-1;
    //need =true;
      attach=false;
  }
      int k=0;
      
      while(args[count]!=NULL && hit !=1){
	
	  if (strcmp(args[count],target1)!=0 && strcmp(args[count],"|")!=0 ){
		command1[k]=args[count];
	     
	      k++;	
	    
	  }
		else if(strcmp(args[count],target1)==0 ){
		  
		  count++;
		   if (track ==0 && strcmp(args[count],"|")==0){
		    hit =1;
		    count++;
		  break;
		     
		  }  
		  
		  
		}
	  else if (strcmp(args[count],"|") == 0){
		hit =1;
		  count++;
		 break;
		}
	   
	    count++;	
		
	}
	command1[k]=NULL;
      
	
      
     
	  int d=0;
	  hit=0;
	  while(args[count]!=NULL && hit!=1&& num_cmds==3){
	 if (strcmp(args[count],"|") == 0){
		hit =1;
		 count++;
		
		 break;
		}
	command3[d]=args[count];
	count++;
	
	d++;
	 }
	command3[d]=NULL;  
	
	  
	//The last command  
	int c=0;
	hit =0;
	
	while(args[count]!=NULL && hit !=1){
	
	  if (strcmp(args[count],target2)!=0 && strcmp(args[count],"|")!=0 ){
		command2[c]=args[count];
		//printf("The inpput command %s, and i %d\n",command2[c],c);
		c++;
	    
	  }
		if(strcmp(args[count],target2)==0 ){
		  count++;
		  if(args[count]==NULL){
		  
		    break;
		  }
		 
		}
	 if (strcmp(args[count],"|") == 0){
		hit =1;
		 count++;
		 break;
		}
	
	count++;
	}
	command2[c]=NULL;
	
	
	
	if (pipe(filedes) < 0) {
      perror("Error creating pipe");
      exit(-1);
      }
	if (pipe(filedes2) < 0) {
      perror("Error creating pipe");
      exit(-1);
      }
	pid=fork();
	if(pid==0){
	
	//printf("THis is the child1\n");
	close(1);
	if(opt==1 ||opt ==2){
	int filedescription1;
	  filedescription1 = open(in, O_RDONLY,0664);  
			if(filedescription1<0){
			printf("Not found file\n");
			printf("%d The err\n",err);
			exit(0);  
			}
			//close(0);
			// We replace de standard input with the appropriate file
			dup2(filedescription1, 0);
			close(filedescription1);
	}
	
	
	dup2(filedes[1],1);
	
	close(filedes[1]);
	close(filedes2[0]);
	close(filedes2[1]);
	close(filedes[0]);
	
	  if ((execvp(command1[0],command1))<0) {
	  perror("Execvp ls failed");
	  exit(-1);
	}
	exit(-2);
	}
	 close(filedes[1]);
	
	 
	 //3 commands
	if(num_cmds==3){
	
	/*
	if (pipe(filedes2) < 0) {
      perror("Error creating pipe");
      exit(-1);
      }
	*/
	pid2=fork();
	if(pid2==0){
	
	close(0);
	dup2(filedes[0],0);
	
	close(1);
	dup2(filedes2[1],1);
	
	close(filedes[0]);
	close(filedes2[1]);
	close(filedes[1]);
	close(filedes2[0]);
	
	 if ((execvp(command3[0],command3))<0) {
	  perror("Execvp ls failed");
	  exit(-1);
	}
	exit(-2);
	  
	}
	close(filedes[0]);
	close(filedes2[1]);
	 
	
	  pid3=fork();
	int filedescription=0;
	if(pid3==0){
	close(0);
	
	
	if(opt==0|| opt==2){
	filedescription = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0664); 
			
			// Replace the standard output with the appropriate file
			dup2(filedescription,1); 
			close(filedescription);
	  
	}
	dup2(filedes2[0],0);
	
	close(filedes2[0]);
	close(filedes[0]);
	close(filedes2[1]);
	close(filedes[1]);
	
	  if ((execvp(command2[0],command2))<0) {
	  perror("Execvp ls failed");
	  exit(-1);
	}
	exit(-2);
	}
	 close(filedes2[0]);
	 //close(filedescription);
	}	 
	 
	 
	 //Only 2 commands
	else if(num_cmds==2){ 
	pid2=fork();
	
	if(pid2==0){
	

	close(0);
	
	dup2(filedes[0],0);

	
	if(opt==0|| opt==2){
	int filedescription1=0;
	  filedescription1 = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0664); 
			
			// Replace the standard output with the appropriate file
			dup2(filedescription1, 1); 
			close(filedescription1);
	  
	}

	close(filedes[0]);
	
	  if ((execvp(command2[0],command2))<0) {
	  perror("Execvp ls failed");
	  exit(-1);
	}
	exit(-2);
	}
	
	 close(filedes[0]);
	
	  
	}
	   
	  //Parent
	 int exitStatusA, exitStatusB, exitStatusC,x;
	x=wait(&exitStatusA);
	printf("PID %5d exited, status = %d\n",x,exitStatusA);
	x=wait(&exitStatusB);
	printf("PID %5d exited, status = %d\n",x,exitStatusB);
	if(num_cmds==3){
	x=wait(&exitStatusC);
	printf("PID %5d exited, status = %d\n", x,exitStatusC);
	}
		
}

void printToken(char *token){
int i = 0;
while (token != NULL){
printf ("[%d] %s\n",i, token);
token = strtok (NULL, " ");
i++;
  
}
i=0;
}

