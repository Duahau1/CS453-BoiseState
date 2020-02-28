# Project Number 3/Threadsafe Library

* Author: Van Nguyen
* Class: CS453 [Operating Systems] Section #001

## Overview

Create a thread safe version of the given list library and test the library against the producer/consumer problem.

## Manifest

ThreadsafeBoundedList.c - The wrapper class for creating the threadsafe library
README.md - Documentation of this project

## Building the project
First you must export the library using the following command:
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../list/lib:./lib
Then run 
$make
Then 
$ ./pc [poolsize] [items] [#num of producers] [#num of consumers] [sleeptime]

## Features and usage
This code create a threadsafe library to make sure that any program that use the library to ensure only 1 thread to enter critical section .

## Testing

I used the test file provided by the project and passed all of them. I also ran the tests with the command line to make sure they work. For example, I ran the command line $./pc 10 10 1 2 10 and all of them passed as well.

### Valgrind
I only have reachable memory leak just like the last projects with no indirect memory leak or completely lost memory error.

### Known Bugs

I did not have any known bugs.

## Reflection and Self Assessment

This project went really smooth for me since I did not have a lot of challenges. I think the only problem I got was mallocing the space for the struct. I did not remember a lot about struct so I got some
problem when mallocing its space and ran into segment fault multiple times. I look up some material online(mostly from Stackoverflow.com) and fixed the problem. The book helps a lot in this project adn pretty much all the code in this project come from different library that was available to me. The thing that clicked me from this project is the ability to use lock especially how to mallocing for a struct. 

## Sources used

Instructor provided code, past slides in CS253, Stackoverflow.com for dealing with segmentation dump and aborted core dump.
