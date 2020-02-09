/*
  This program needs the file error.c and mydash.h to compile.
 */
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <mydash.h>
#include    <readline/readline.h>
#include    <readline/history.h>
#include    <pwd.h>
#include    <unistd.h>

#define MAXLINE 4096
void changeDir(char **token);
const char* git_version(void);
char** tokenizeLine(char *line);
void freeArray(char **array);
/** Main program that is running
* @args: argc, *argv[]
*/

int main(int argc, char *argv[])
{
  
    pid_t   pid;
    int     status;
    char    *line;    
    char    **cmd;    
    int     numTokens;
    char    *DASH_PROMPT;


    if(  argc==2 && strcmp(argv[1],"-v")==0)
    {
      printf("mydash: Version 1: Revision %s (author: VanNguyen599@onyx.boisestate.edu)\n", git_version());
      exit(0);
    } 
    else if (argc==2 && strcmp(argv[1],"-v")!=0)
    {
      printf("Invalid Option \n");
      exit(0);
    }
    char *env = getenv("DASH_PROMPT");
  
    numTokens = 0;    // the number of tokens in 1 command line
     
    if (env == NULL){

      DASH_PROMPT =  "mydash>";
    }
    else
    {
      DASH_PROMPT = env;
    }
    using_history(); 
    while ((line=readline(DASH_PROMPT)))
    {
        // Case 1: The line is empty
        if (line[0] == '\0') 
        {
          continue;
        }     
        // Case 2: The line is not empty
        add_history(line);
        
        // Case 2a: The command is "exit"
        if(strcmp(line,"exit")==0)
        {         
          exit(0);
        }
        cmd = tokenizeLine(line);
        // Case 2b: The command is "cd"
        if(strcmp(cmd[0],"cd")==0)
        {
          changeDir(cmd);  
          free(line);
          continue;
        }

        //Case 3 : Linux like command

        if (strcmp(cmd[0], "-v") == 0) 
        {
          printf("Version: %s \n", git_version());
          free(line);
          break;
	      }
        pid = fork();
        if(pid==-1)
        {
          err_ret("couldn't execute: %s", line);
          exit(EXIT_FAILURE);
        }
         else if (pid == 0) 
         { 
          if (execvp(cmd[0],cmd)==-1)
          {   
            err_ret("couldn't execute: %s", *cmd);
            exit(1);
          }
         freeArray(cmd);
        }
        freeArray(cmd);
        if ( (pid = waitpid(pid, &status, 0)) < 0)
        {
            err_sys("waitpid error");
            free(line);
            freeArray(cmd);
        }
    free(line);
    }
    exit(0);
}

/** changDir: Change directory when typed "cd"
* @args: ** token to change directory base on the command line
*/

void changeDir(char **token)
{
  if (token[1])
  {
     int success;
     success=chdir(token[1]);
     if(success!=0)
     {
       err_ret("error:%s does not exist ",token[1]);  
     } 
  }
  else
  {
    char *homedir = getenv("HOME");
    if (homedir == NULL)
    {
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        char *homedir = pw->pw_dir;
        chdir(homedir);
    }
    else
    {
        chdir(homedir);
    }
  }
  freeArray(token);
}

/** tokenizeLine: break command line into token and return array with null token at the end and all white space removed
* @args: * line to break into tokens @return tokenized line stored in array input
*/

char** tokenizeLine(char *line) {
    char *nextToken;
    int i=0;
    char **input;
    char *token;
    char *temp;

    temp = malloc(strlen(line) * sizeof(char) + 1);
    strcpy(temp, line);
    input = malloc(MAXLINE * sizeof(char*));
    nextToken = strtok(temp, " ");
    
    while (nextToken != NULL) 
    {
      input[i] = malloc(strlen(nextToken) + 1);
      token = input[i++];
      strcpy(token, nextToken);
      nextToken = strtok(NULL, " ");
    }
    
    input[i] = '\0';
    free(temp);
    return input;
}

/** freeArray: free slot by slot in double pointer
* @args: double pointer that needs to be freed
*/
void freeArray(char **array) {

  int i = 0;
  while (array[i] != NULL) 
  {
    free(array[i]);
    i++;
  }    
  free(array);
}


/* vim: set ts=4: */
