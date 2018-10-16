#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define MAXLINE 4096



int main()
{
char buff1[MAXLINE];
char buff2[MAXLINE];
fprintf(stderr,"$");
//Print $
while (fgets(buff1, MAXLINE, stdin) != NULL) {
if (strlen(buff1)==1){
fprintf(stderr,"$");
}
else {
int count=strlen(buff1);
count--;
buff1[count]='\0'; //replace newline with NULL

strncpy(buff2,buff1,MAXLINE);

char *token = strtok(buff2," ");

if (strcmp("exit",token)==0) {
 exit(0);
}
if(strcmp("cd",token)==0){

token=strtok(NULL," ");
int dir=chdir(token);
char buffer[MAXLINE];
if(dir ==0){
char* dirch = getcwd(buffer,sizeof(buffer));
  
printf("%s\n", dirch);  
}
else{
printf("error: ");
printf(token);
printf(" does not exist\n");
}
  
}
else {
char* token = strtok(buff1," ");
int i = 0;
while (token != NULL){
printf ("[%d] %s\n",i, token);
token = strtok (NULL, " ");
i++;
}  

  
}
fprintf(stderr,"$");
}
}

return 0;
}
