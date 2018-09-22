#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>



static bool do_line=false;
static bool do_word=false;
static bool do_byte=false;
static bool check =false;
static int w=0;
static int l=0;
static int ch=0;
void usage(){
  printf("%s\n","Usage: ./wc253 file");
  printf("%s\n","Usage: ./wc253 file [-l|-w|-c]");
  
}

void fileExists(char *filename)
{
   FILE *file = fopen(filename, "r");
   if (file == NULL){ 
    check =false;
  }
   else{
    check=true;
   fclose(file);
   }
}
//to check if the file exist
// words count, lines count and bytes count
void Counting(char *checkedFile){
FILE *file= fopen(checkedFile,"r");

int c;
bool isInside =false;
int wc;
int by;
int lc;
wc =0;
lc=0;
by=0;
while ((c=fgetc(file)) != EOF){
//word count 
if(c==' '||c=='\n'||c=='\t'){
isInside= false;
}
else if(isInside==false){
isInside=true;
++wc;
}
// line count
if(c=='\n'){
++lc;
    }
//byte count
++by;
}
 
 if(do_word==true){
 w=wc;
   //printf("%d",wc );
 //printf(" ");
   
}
 if(do_line==true){
 l=lc;
   //printf("%d",lc); 	
   //printf(" ");   
   
}
 if(do_byte==true){
 ch=by;
   //printf("%d",by); 
  //printf(" ");   
   
}
 fclose(file);  
//return 0;
  
}


void fileExists(char *filename);
void Counting(char *checkedFile);
void usage();


int main(int argc, char **argv)
{
  /*
  **Put your code here. You should create as many functions 
  **as necessary to create a modular program. We should not 
  **see ALL your code in the main function! 
  **
  **See ExampleGetopt and ExampleCharacterIO for examples of
  **processing command-line options and character I/O from
  **a file.
  */

 if(argc>3 ||argc<2){
 usage();
   return 1;
}
fileExists(argv[1]);
if (check== false){
  printf("%s","File not found\n");
  return 1;
}

char option;
while( (option=getopt(argc, argv, "wlc")) != -1) {

switch(option){

  
case'w':
  do_word=true;
  break;

case'l':
  do_line=true;
  break;

case 'c':
do_byte=true;

break;

case '?':
fprintf(stderr,"Unknown option -%c\n",optopt);
return 1;
break;

default:
  usage();
return 1;
  
}
Counting(argv[1]);
} 
if(do_word){
printf(" %d ",w);
  
}
if(do_line){
printf(" %d ",l);
  
}
if(do_byte){

  printf(" %d ",ch);
}

if (!do_word && ! do_line && ! do_byte){
      do_line = do_word = do_byte = true;
      Counting(argv[1]);
      printf(" %d %d %d ",w,l,ch);
    }
      

for (int index = optind; index < argc ; index++){
printf (argv[index]);
}
printf("\n");
return 0;  //Error exit status
  
}

