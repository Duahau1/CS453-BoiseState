## Project Number 4/Device Driver

* Author: Van Nguyen
* Class: CS453 [Operating Systems] Section #001

## Overview
Create a simple character driver called booga that prints out a random string with specified port with read options. 

## Manifest
booga.c - The main class that does the I/O part with semaphores
booga.h - The header file for booga.c
README.md- Documentation of this project
backpack.sh - Test Plan

## Building the project
In order to run this project, first you have to run the $make clean command just in case some leftover artifacts are remained. Next, you run $make to compile the whole thing. Then you run $sudo ./booga_load and run command with $./test-booga [port] [nbyte] [operations], for example:
$./test-booga 0 100 read. Then when you finish using the driver, all you need to do is type $ sudo ./booga_unload. 

## Features and usage
This program is basically attaching a simple driver to the Centos 7 OS so that you can interact with it by simple reading/ writing operations. 

## Testing
I test this project with the given booga-test1.sh and booga-test2.sh in the VM environment but I got the result ' rmmod: ERROR: Module booga is in use' to which when I took a look at those 2 files and I realized that for running both the load and unload files they did not use the sudo keyword. I added the keyword to run the tests and passed everything. Towards the end of booga-test1.sh there were tests where you compare the result of your running test-booga.c with the wc command line to which I found out that my results were always 1 larger than the given number of byte. The problem to that weird results come from the test-booga.c file where everytime it prints out the string, it appends /n at the end and since we are not able to modify test-booga.c so I can say that my results are correct. Additionally, I also wrote a lot of test cases for the my project as listed(they are under ./bacpack.sh).
./test-booga 0 100 read
./test-booga 1 100 read
./test-booga 2 100 read
./test-booga 3 100 read
./test-booga 0 200 read
./test-booga 1 200 read
./test-booga 2 200 read
./test-booga 3 200 read
./test-booga 0 100 write
./test-booga 1 100 write
./test-booga 2 100 write
./test-booga 3 100 write
./test-booga 0 1000 write
./test-booga 1 1000 write
./test-booga 2 1000 write
./test-booga 3 1000 write
My test plan would be testing for both read, write using the backpack.sh, normal functionality and semaphore using given test files to make sure that they cannot write to dev/booga3 and the length of read matches the wc.

## Valgrind
I only have reachable memory leak just like the last projects with no indirect memory leak or completely lost memory error. I also got some heap memory lost. For example, when I ran [cs453@localhost p4]$ valgrind --leak-check=full ./test-booga 0 100 write , I got 
 in use at exit: 20 bytes in 1 blocks
 total heap usage: 2 allocs, 1 frees, 121 bytes allocated

## Known Bugs
I did not have any known bugs.

## Reflection and Self Assessment
This project was not super challenging even though it was quite hard to understand all the code in the example driver class. It took me nearly 8 hours straight just to understand the code and 2 hours to set up the VM environment because we are running in the Kernel mode so whenever I wanted to put something down I had to Google to see its usage but it was  hard to read those documentation. When I wrote this project my VM got hangs so many times so it was quite frustrating to test it. Since it was just hanging without giving me any error, debuging this project took quite long time to. One of the problem I had was the proc_create() where it got me error so many times as I passed the pointer instead of the address and I did not see the problem as my machine just hanged. I fixed it by comparing the known parts with the example driver code.The thing that clicked me from this project is the ability to use semaphore as lock.

## Sources used
Instructor provided code(example driver code), past slides in CS253, Stackoverflow.com for dealing with segmentation dump and aborted core dump.