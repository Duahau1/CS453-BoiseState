#!/bin/bash
#-------------------------------------------------------------------------------
# NAME
#   autograder.sh -- Grades the smash2 project awarding partial credit
#
# SYNOPSIS
#   backpack.sh outputFile
#
# DESCRIPTION
#   This script grades the CS253 "smash" Part2 project, awarding partial
#   credit.  The script keeps a running total of a student's score, and, after
#   a problem is encountered, attempts to grade whatever else might work.  
#
#   The script records feedback to the student in the specified file in a
#   format that can also be processed by the "backpack -g" auto-grader command
#   (which requires the file be named rubric.txt).
#
#   Most messages written by the script include the line-number of their origin:
#     [42] Foo Fi Fo Fum I smell the blood of an Englishman
#   in brackets, [42].  If you open the script in an editor, you can examine
#   around the line of their origin to investigate what is happening there.
#
# RESOURCES
#   test.in         Regression test of Smash Part1 functionality
#   test.out        Expected output from test.in
#   basicHistory.in Test data for the history command
#
# HISTORY
#   03/08/2018 Epoch..........................................................jrc
#--------------------------------------------------------------------------------
if [ "$1" = "" ]; then
  echo "usage: $0 outputfile"
  exit 0;
fi

#Configuration variables you can easily modify
let maxscore=80                  #The maximum score allotted for this assignment
errMsg=ErrorsAndWarnings.txt     #Where to record stderr messages
EXE=smash                        #Name of the student's executable program

#Configuration that depends upon host operating system.  Note:  MacOS ships without
#the timeout utility, but gtimeout can be installed with "brew install coreutils"
#after downloading/installing MacOs brew from https://brew.sh (or just use linux)
timeout=timeout                  #Use timeout on Linux, gtimeout on MacOS

#Initialization
let letterGrade=maxscore/10      #Points in one letter-grade for this assignment
let halfLetterGrade=letterGrade/2
dest="$1"                        #The output log file of grading activity/messages
let score=$maxscore              #This student's calculated score
tmpFile=/tmp/"$USER""$$"         #Temp file sometimes used as a scratchpad

#The autograder's required data files containing test input and output
testIn=test.in                   #Test input data file
testOut=test.out                 #Test expected output file
historyIn=basicHistory.in        #Commands for exercising history command
bigIn=big.in                     #A very long illegal command followed by exit

#---------------------------------------------------------------------------------
# Define function to log a message to the dest file, stamped with the line number
# of where the function was invoked, pointing students to where problems arose.
# Usage:  logMsg $LINENO messageText
#---------------------------------------------------------------------------------
logMsg() {
    echo "[$1] " "$2" " -- score=$score" >>$dest
}


#---------------------------------------------------------------------------------
# Define function to deduct points if the specified file exists
# Usage:  deductFileExists $LINENO filename nPoints
#---------------------------------------------------------------------------------
deductFileExists() {
    if [ -e "$2" ]; then
        deductPoints $3
        logMsg "$1" "ERROR:  Unexpected file $2"
    fi
}


#---------------------------------------------------------------------------------
# Define function to deduct points if the specified file is missing
# Usage:  deductFileMissing $LINENO filename nPoints
#---------------------------------------------------------------------------------
deductFileMissing() {
    if [ ! -e "$2" ]; then
        deductPoints $3
        logMsg "$1" "ERROR:  Missing expected file $2"
    fi
}


#---------------------------------------------------------------------------------
# Warn user if a required file is missing (sometimes someone deletes them)
# Usage:  warnFileMissing $LINENO filename
#---------------------------------------------------------------------------------
warnFileMissing() {
    if [ ! -e "$2" ]; then
        logMsg "$1" "WARNING:  Missing expected file $2 --- autograder will likely fail"
    fi
}

#---------------------------------------------------------------------------------
#Define function to deduct points if pattern is missing from a specified file
# Usage:  deductnGrep $LINENO pattern filename nPoints
#---------------------------------------------------------------------------------
deductnGrep() {
    output=$(grep -i "$2" "$3")
    if [ "$?" != 0 ]; then
        deductPoints $4
        logMsg "$1" "ERROR:  Could not grep $2 in $3"
    fi
}


#--------------------------------------------------------------------------------
# Define function to deduct points if pattern is found in the specified file
# Usage:  deductGrep $LINENO pattern filename nPoints
#--------------------------------------------------------------------------------
deductGrep() {
    output=$(grep -i "$2" "$3")
    if [ "$?" == 0 ]; then
        deductPoints $4
        logMsg "$1" "ERROR:  grep found unexpected $2 in $3"
    fi
}

#---------------------------------------------------------------------------------
# Define function to deduct points if substring is missing from string ignoring
# case.
# Usage:  deductMissingString $LINENO string substring nPoints
#---------------------------------------------------------------------------------
deductMissingString() {
    if ! echo "$2" | fgrep -i -q "$3" ; then
        deductPoints $4
        logMsg "$1" " ERROR:  Substring $3 missing in string, $2"
    fi
}

#---------------------------------------------------------------------------------
# Define function to deduct points from score
# Usage:  deductPoints nPoints
#---------------------------------------------------------------------------------
deductPoints() {
    let score-=$1
    if [ $score -lt 0 ]; then
        let score=0
    fi
}


#---------------------------------------------------------------------------------
#NOTE:  Grading begins here
#---------------------------------------------------------------------------------
rm -f $dest                                         #Remove old grading artifacts
logMsg $LINENO "NOTE:  Begin grading on `date`"
logMsg $LINENO "NOTE:  Using temp file, $tmpFile"

#Check for missing autograder files
warnFileMissing $LINENO $testIn
warnFileMissing $LINENO $testOut
warnFileMissing $LINENO $historyIn

#Deduct points for submission of binary files-------------------------------------
#Note:  Students should do a make clean before running autograder
logMsg $LINENO "NOTE:  Checking for binary files"
deductFileExists $LINENO $EXE $halfLetterGrade
deductFileExists $LINENO smash.o $halfLetterGrade

#Verify the expected Makefile
logMsg $LINENO "NOTE:  Grading the Makefile"
deductFileMissing $LINENO  Makefile $halfLetterGrade
deductnGrep $LINENO "Wall" Makefile $halfLetterGrade
deductnGrep $LINENO "c99"  Makefile $halfLetterGrade

#Manually clean old junk from previous builds and grading attempts
rm -f *.o *.d $errMsg diff.out

#Build the product and check for errors/warnings--------------------------------
logMsg $LINENO "Note:  Grading the build"
make 2>$tmpFile
deductGrep $LINENO error   $tmpFile $letterGrade
deductGrep $LINENO warning $tmpFile $halfLetterGrade
deductFileMissing $LINENO $EXE $maxscore

#Verify program will run given no input except an EOF----------------------------
logMsg $LINENO "Note:  Grading smash exits normally with EOF input"
./$EXE </dev/null >>$errMsg 2>&1
if [ "$?" != 0 ]; then
    deductPoints $letterGrade
    logMsg $LINENO "ERROR: $EXE exited abnormally after EOF"
fi

#Verify smash supports the exit command and exits with status=0=-----------------
logMsg $LINENO "Note:  Grading the exit command"
echo exit | $timeout 5s ./$EXE >>$errMsg 2>&1
if [ "$?" != 0 ]; then
    deductPoints $letterGrade
    logMsg $LINENO "ERROR: exit command timed-out or exited with non-zero status"
fi

#Verify smash can change to user's home directory---------------------------------
logMsg $LINENO "Note:  Grading the cd command"
result=$( echo "cd $HOME" | $timeout 5s ./$EXE )
deductMissingString $LINENO "$result" "$HOME" $letterGrade

#Verify smash emits an error message for cd to a non-existent directory----------
logMsg $LINENO "Note:  Grading stderr message for cd to a non-existent directory"
echo "cd non-existent-directory" | $timeout 5s ./$EXE >$tmpFile 2>&1
deductnGrep $LINENO "Error" $tmpFile $letterGrade

#Verify smash can handle long commands
logMsg $LINENO "Note:  Grading a really long invalid command"
$timeout 5s ./$EXE <$bigIn >$tmpFile 2>>$errMsg
if [ "$?" != 0 ]; then
    deductPoints $letterGrade
    logMsg $LINENO "ERROR: Something went wrong processing a long command, $bigIn"
fi

#Verify that smash is parsing those tokens per the assignment--------------------
#Note:  The diff command is comparing smash results in $tmpFile for the input
#from test.in with the expected output found in test.out.  To the extent possible,
#the diff is configured to ignore silly differences (blank lines, white space,
#upper vs. lower case).  If your implementation fails the test below, then the
#likely culprit is your output doesn't *exactly* match that requested by the assignment.
#Note:  The diff output is placed in the file, diff.out, and later deleted if
#all goes well.
logMsg $LINENO "Note:  Grading token parsing per the assignment specification"
$timeout 5s ./$EXE <$testIn >$tmpFile 2>>$errMsg
diff -i -w -B $testOut $tmpFile >>diff.out
if [ "$?" == 0 ];then
    rm diff.out           #Delete if not needed for debugging students program
else
    deductPoints $letterGrade*2
    logMsg $LINENO "ERROR:  smash is not parsing tokens from file $testIn per assignment"
    logMsg $LINENO "Note:  The expected output is in $testOut"
    logMsg $LINENO "Note:  The actual output is in $tmpFile"
    logMsg $LINENO "Note:  The diff result is in diff.out"
fi



#Verify function names required by the assignment appear in the history.c sources
logMsg $LINENO "Note:  Grading required function names in history.c"
deductnGrep $LINENO init_history history.c $halfLetterGrade
deductnGrep $LINENO add_history  history.c $halfLetterGrade
deductnGrep $LINENO clear_history history.c $halfLetterGrade
deductnGrep $LINENO print_history history.c $halfLetterGrade

#Verify the basic functionality of the history command
logMsg $LINENO "Note:  Grading history command"
timeout 5s ./$EXE <basicHistory.in >$tmpFile 2>>$errMsg
deductnGrep $LINENO "0.*ls"      $tmpFile $halfLetterGrade
deductnGrep $LINENO "1.*cd"      $tmpFile $halfLetterGrade
deductnGrep $LINENO "2.*pwd"     $tmpFile $halfLetterGrade
deductnGrep $LINENO "3.*history" $tmpFile $halfLetterGrade
deductnGrep $LINENO "4.*history" $tmpFile $halfLetterGrade

#Verify that valgrind finds nothing of extreme significance
logMsg $LINENO "Note:  Grading valgrind results"
which valgrind
if [ "$?" != 0 ]; then
    logMsg $LINENO "WARNING:  valgrind not installed/found on this computer"
fi
timeout 10s valgrind ./$EXE <basicHistory.in >$tmpFile 2>&1
deductGrep $LINENO "Invalid\\s*write"         $tmpFile $letterGrade
deductGrep $LINENO "definitely lost\\s*[1-8]" $tmpFile $letterGrade

#Grade make clean-----------------------------------------------------------------
logMsg $LINENO "Note:  Grading make clean"
make clean
if [ "$?" != 0 ]; then
    deductPoints $letterGrade
    logMsg $LINENO "ERROR: make clean did not exit normally"
fi
deductFileExists $LINENO smash   $halfLetterGrade
deductFileExists $LINENO smash.o $halfLetterGrade

#Record the score and output to console--------------------------------------------
logMsg $LINENO "NOTE:  Finished grading"
echo "Grade:  $score" >>$dest                          #backpack -e needs this
echo "Grading finished.  Results are in file, $dest"
