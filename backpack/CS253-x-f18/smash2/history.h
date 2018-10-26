#ifndef __MYHEADER_H
#define __MYHEADER_H

#include<stdio.h>
#include<string.h>
#include<unistd.h>

struct Cmd {
char * cmd; //Character pointer referencing the user’s command string

};

struct hist{
int index;
struct Cmd **cmdArray;
  
};
struct hist* init_history();
void add_history(struct hist * ,char *);

void clear_history(struct hist *);

void print_history(struct hist *);

#endif /* __MYHEADER_H */
