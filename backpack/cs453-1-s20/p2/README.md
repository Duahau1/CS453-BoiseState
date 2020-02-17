# Project Number 2 / MergeSort using Multiple Threading

* Autho Van Nguyen
* Class: CS453 [Operating Systems] Section #01

## Overview
This project apply multiple threading to mergesort a big set.  


## Manifest

Makefile - File to build
mergesort.c - Mergesort using multiple threads
mergesortTest.c - Driver class to run the program that parse commandline 
README.md - Documentation of this project

## Building the project

In order to run the code: $make clean then $make
if you want to see the usage just type $./mergesort 
./mergesort <input size> <number of threads> [<seed>]
Example: ./mergesort 100000000 8 2


## Features and usage

This program will generate a set of number and sort through them using mergesort. In order to specify how many number you want to generate, change the input size. Change the number of threads run to boost the sorting process.

## Testing

This section should detail how you tested your code. Simply stating "I ran
it a few times and it seems to work" is not sufficient. Your testing needs to
be detailed here or even better, this section should document the appropriate
smoke/unit/system tests that you wrote.

I ran my code with an increament of 1 from 1 to 8 threads and achieve the table as below.
The input size is  100,000,000

Number of Threads | #1    | #2   | #3   | #4    | #5    | #6    | #7     | #8     |
---               |-------|------|------|-------|-------|------ |--------|--------|
Seconds           | 15.88 | 8.98 | 5.42 | 3.40  | 2.40  | 1.93  | 1.99   |  2.12  | 

Comments: We can see that the computed time is halved when the number of threads increase which indicates that my program is running well.
CPU information Processor:Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz, 1800 Mhz, 4 Core(s), 8 Logical Processor(s)


### Valgrind
I ran valgrind and still have some memory leak that were still reachable. I came to ask professor about the issue and he said it was fine to have that type of memory leak.

### Known Bugs

I did not have any known bugs. 

## Reflection and Self Assessment

I encountered a lot of problems while implementing this project. First of all, I did not know where to start and I did not understand how the program was going to function. It took me 5 hours just figuring where to start. I did some researches on Google and I found the pthread library and they were so nice that they offered some examples upthere. Other things I struggle was with the recursive method as I ought to implement and how pass the information as well as modify the information; that's when I decided to use struct and I struggle with it to as I did not use struct in the past. I had to go over the book that I studied in CS253 to learn how to create and access the struct again. I got all kinds of possible errors namely segmentation dump, malloc error also, and memory leak,... I fixed them by implemented the program little by little as well as placing printf everywhere. 

I found the part that using create pthread challengind. Things that would click after this project is the ability to use struct, how to create and use threads. The testing and developing process went really slow for me, I spent 10 hours on this project on estimation. 

## Sources used
Instructor provided code, past slides in CS253, Stackoverflow.com for dealing with segmentation dump and aborted core dump.

