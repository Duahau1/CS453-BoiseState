# Project Number/Title {#mainpage}

* Author: Van Nguyen
* Class: CS453 [Operating Systems] Section #001

## Overview

The program is basically a simple shell that has the ability accept arguments to commands and has built in basic functionality namely exit and cd. 

## Manifest

1. mydash.c - the file where mydash shell live.
2. mydash.h - header file for the shell.
3. README.md - Documentation of the project.
4. Makefile - File that build the project. 
5. valgrind.supp - The given file to suppress all the errors.
6. TestCases     - The test plan

## Building the project

$make all -- build the whole shell
$make mydash-project -- build the whole shell
$make clean -- remove the project build artifacts

## Features and usage

Given the cd command line,if the following argument is a valid path of directory, your working directory will be changed. However, if the directory directory is inaccurate, error will be printed out saying whatever you have requested does not exist.Additionally, if you just type in "cd", it will take you back to the HOME directory. 

Given the exit command line or Ctrl-D, the shell will be terminated.

Given other arguments apart from those 2 command lines,if the given command line is valid (as those in the Linux environment), the shell will execute them the same way as Linux does. 

Given invalid command lines,the program will print out a message saying that the input is not valid and prompt you for a new one.  

Given no arguments, the program will print out mydash> until either you terminate the program or put in any arguments.

When the shell is started with a command line argument -v, it prints out the version of the project and then quits.

Checks for an environment variable DASH_PROMPT. If the environment variable is set, then it uses the value as the prompt. Set the environment variable using the export command in the terminal, as shown: export DASH_PROMPT=”myprompt>”

## Testing

I tested it using by adding some of my test cases to the smoketestfiles folder and modify the backpack.sh file. Also I ran my test with various Linux command like pwd, wc, ps, locate, cat, find in order to test its functionality. All the tests will be in the TestCases file.
I have manually tested all the planned test in the TestCases file as well as run all the smoke test to make sure that my program ran as expected. 

### Valgrind
After quite a few hours spent on checking memory leak with valgrind, I have sucessfully free all the bytes that are considered definetely lost or indirectly lost; still, I am left with a huge number of memory leak that is reachable. However, after some researches I found out that those memory leak was caused by third party who created the readline library and there is nothing that could be done to improve it.

### Known Bugs

I did not have any known bugs. 

## Reflection and Self Assessment

I got a lot of segmentation dump over the place as I wrote this program. I got a problem when I was testing on ls and pwd. I first start off with using $ls -a and then after that I type $pwd and I got an error saying that my command was not valid. I then figured it out it was because I did not free each of the element in the cmd double pointer, I came back and free each of it- problem solved. I also encountered aborted core dump which took me sometimes to research to fix it, which got me to realize that I free the variable line 2 times.

I realized that I understand about environment variables more than I do. The challenge is dealing with memory leak when using readline. 

## Sources used
Instructor provided code, Past Projects in CS253(smash shell), Stackoverflow.com for dealing with segmentation dump and aborted core dump.
