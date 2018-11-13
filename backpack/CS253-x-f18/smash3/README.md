##NAME
smash -- A Simplified Shell

##SYNOPSIS

make    -- To build the smash shell

make all   -- To build the smash shell

make smash -- To build the smash shell

make clean -- To clean all the build artifacts

make debug -- To build a debug version of the smash shell

make valgrind -- To build a debug version and executes the product under valgrind
 
./smash -- To start the smash shell

cd [Directory Path] -- To change the working directory 

exit -- To terminate the smash shell

history -- To print out all the captured commands with the capacity of 100 command lines 

Valid Linux Command -- To execute all the commands the same as the linux environement

##DESCRIPTION

First, you should use $make clean comand to clear all the unecessary files. Then, use the $make command to create an executable 
smash. If you want to check for memory leak or run in debugging mode, type in $make valgrind or $make debug respectively.
Once you have done building your executable file smash, use ./smash to run the shell. At this point in time, this smash shell only supports 3 commands: cd, exit and history. 

If you want to move between directories, type cd and gives it a valid path; or if you want to terminate the shell script,type exit.
Otherwise, typing anything and press enter will just print out your argument lines with each tokens being numbered.

Given the cd command line,if the following argument is a valid path of directory, your working directory will be changed (the current
working will be printed out in the console). However, if the directory directory is inaccurate, error will be printed out saying whateveryou have requested does not exist.Additionally, if you just type in "cd", the command will print out error saying that you should enter apath name. 

Given the exit command line, your smash shell will be terminated.

Given the history command line, the shell will print out whatever commands line you have put in the console before including the 
history command you just used. After the history command reaches its capacity which is 100, it will refresh itself and begin to
store new command lines.

Given other arguments apart from those 3 command lines,if the given command line is valid (as those in the Linux environment), smash shell will execute them the same way as Linux does. 

Given invalid command lines,the program will print out a message saying that the input is not valid and prompt you for a new one.  

Given no arguments, the program will print out $ until either you terminate the program or put in any arguments.


##FILES
* README.md -- The only starter file containing instructions for using smash
* Makefile  -- The file that builds executable file, clear files
* smash.c   -- The source code for the smash project
* history.h -- The header file that contains function prototypes and structs that are implemented in history.c
* history.c -- The source code that implements all the back-end of the history command line
* backpack.sh -- Backpack's smash2 autograder
* test.in -- Test data used by autograder
* test.out -- The expected output given that test data
* basicHistory.in -- The test data used by the auto grader
* big.in --   The test data used by the auto grader
* zombieCheck --The test data for hunting zombies

##INSTALLATION

Makefile is used to build the smash project

TARGETS
* all        -- Build everything without debugging
* smash      -- Build the smash project
* debug      -- Build a debugging version
* valgrind   -- Build debbug and then execute under watchful eye of valgrind
* clean      -- Remove build artifacts

PREREQUISITE
* gcc 4.8.5 is used to run make
* valgrind 3.13.0 is used to run valgrind by the Makefile 
* The compiler is executed using the c99 standard, including debugging mode and optimization is disabled (to facilitate debugging/valgrind)

##AUTHORS
Van Nguyen
