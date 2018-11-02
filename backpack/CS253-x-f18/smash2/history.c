#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "history.h"
#define MAX_HIST 100
#define MAX_LINE 4096
 

/*
 * Init the history array with size=100
 */
struct hist* init_history(){
struct hist *history= (struct hist *)malloc(sizeof(struct hist));
history->index=0;
history->cmdArray= (struct Cmd **)malloc(sizeof(char *)*MAX_HIST);
int i=0;
for (i=0;i<MAX_HIST;i++){
history->cmdArray[i]=NULL;
}
return history;
}
/*
 * Add a new command into the history or the cmdArray
 */

void add_history(struct hist* history,char *token){

struct Cmd *newCm=(struct Cmd *)malloc(sizeof(struct Cmd));

if(history->index==MAX_HIST){
int i=0;
for(i=0;i<history->index;++i){
free(history->cmdArray[i]->cmd);
free(history->cmdArray[i]);
//history->cmdArray[i]->cmd=NULL;
//history->cmdArray[i]=NULL;
}
history->index=0;
} 
newCm->cmd =(char *)malloc(sizeof(char*)*MAX_LINE);

strcpy(newCm->cmd,token);
//newCm->cmd=token;

history->cmdArray[history->index]=newCm;
history->index++;  
}
/*
 * Free all the memory and clear the history
 */
void clear_history(struct hist* history){
int i;
for(i=0;i<history->index;++i){
free(history->cmdArray[i]->cmd);
free(history->cmdArray[i]);
//history->cmdArray[i]->cmd=NULL;
//history->cmdArray[i]=NULL;
  
}
  free(history->cmdArray);
  free(history);
 
}
/*
 * Print the history to the command line
 */
void print_history(struct hist* history){
int i;
  for(i=0;i<history->index;++i){

// printf("%d",i);
// printf(" ");
//printf("%s\n", history->cmdArray[i]->cmd);  
printf("[%d] %s\n",i,history->cmdArray[i]->cmd);
    
  } 
  
}
