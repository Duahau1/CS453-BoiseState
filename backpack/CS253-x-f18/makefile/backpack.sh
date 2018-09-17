#!/bin/bash
#-------------------------------------------------------------------------------
# NAME
#   backpack.sh -- Grading script for the CS253 P2 "makefile" project
#
# SYNOPSIS
#   backpack.sh file
#
# DESCRIPTION
#   This grading script is compatible with the backpack -g cmd and must not be
#   renamed to avoid confusion with backpack itself.  The script initially
#   awards full credit and deducts points when problems are encountered.
#   The output is written to the specified file, and contains the detailed
#   log of the grading results for a single student in a format that can
#   be processed by the backpack -e command.  The script must be executed
#   in the directory containing the student's work to be graded.
#
#   The script is also compatible with the backpack -G command, enabling the
#   grading results file to be returned to the student.
#
# HISTORY
#   01/25/2018 Leveraged from 2-tools/projects/makefile/grader/backpack.sh....jrc
#   09/06/2018 Use an initial assumption of full credit.......................jrc
#--------------------------------------------------------------------------------
if [ "$1" = "" ];then
  echo "usage: $0 outputfile"
  exit 0;
fi

#Configuration
let maxscore=30                  #The maximum score allotted for this assignment

#Initialization
dest="$1"                        #The output log file of grading activity/messages
let score=$maxscore              #This student's calculated score
tmpFile=/tmp/"$USER""$$"


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
    let score-=$3
    logMsg "$1" "ERROR:  Unexpected file $2"
  fi
}


#---------------------------------------------------------------------------------
# Define function to deduct points if the specified file is missing
# Usage:  deductFileMissing $LINENO filename nPoints
#---------------------------------------------------------------------------------
deductFileMissing() {
  if [ ! -e "$2" ]; then
    let score-=$3
    logMsg "$1" "ERROR:  Missing expected file $2"
  fi
}



#---------------------------------------------------------------------------------
# Define function to deduct points if substring is missing from string ignoring
# case.
# Usage:  deductMissingString $LINENO string substring nPoints
#---------------------------------------------------------------------------------
deductMissingString() {
   if ! echo "$2" | fgrep -i -q "$3" ; then
      let score-=$4
      logMsg "$1" " ERROR:  Substring $3 missing in string, $2"
   fi
}


#---------------------------------------------------------------------------------
# Grading begins here.  Remove artifacts from previous grading in this directory
#---------------------------------------------------------------------------------
rm -f $dest                                         #Remove old grading artifacts
logMsg $LINENO "NOTE:  Begin grading on `date` using temp file, $tmpFile"

#Verify the expected Makefile name
logMsg $LINENO "NOTE:  Grading expected Makefile name"
deductFileMissing $LINENO Makefile 3

#Verify this submission was cleaned of binaries
logMsg $LINENO "NOTE:  Grading submission cleaned of binaries"
deductFileExists $LINENO main.o 1
deductFileExists $LINENO f1.o 1
deductFileExists $LINENO f2.o 1
deductFileExists $LINENO f3.o 1
deductFileExists $LINENO myprog 1
deductFileExists $LINENO main 1

#Manually clean the student's directory
rm -f *.o *.d myprog main

#Verify make with default target
logMsg $LINENO "NOTE:  Grading make default target"
make
if [ "$?" -gt 0 ] ; then
   let score-=12
   logMsg $LINENO "ERROR:  Make failed to build the default target"
fi

#Verify remake of the default target does nothing
#logMsg $LINENO "NOTE:  Grading remake of default target"
#result=$( make )
#deductMissingString $LINENO "$result" "Nothing to be done" 3

#Verify the executable exists and will run and exit with a good status code
logMsg $LINENO "NOTE:  Grading executable"
deductFileMissing $LINENO myprog $maxscore
./myprog
if [ "$?" -ne 0 ]; then
    let score-=3
    logMsg $LINENO "ERROR:  myprog failed to run and exit with zero status"
fi

#Verify make all builds myprog
logMsg $LINENO "NOTE:  Grading make all"
rm -f *.o *.d myprog main                               #Manually clean old builds
make all
deductFileMissing $LINENO myprog 3

#Verify make myprog builds myprog
logMsg $LINENO "NOTE:  Grading make myprog"
rm -f *.o *.d myprog main                               #Manually clean old builds
make myprog
deductFileMissing $LINENO myprog 3


#Verify Makefile properly detects source file change
logMsg $LINENO "NOTE:  Grading source file changes"
make
touch f1.c
make >$tmpFile
fgrep "f1.c" $tmpFile
if [ "$?" != 0 ]; then
  let score-=6
  logMsg $LINENO "ERROR:  make did not rebuild f1.o after touching f1.c"
fi

#Verify Makefile properly detects header file changes
logMsg $LINENO "NOTE:  Grading header file changes"
make
touch *.h
make >$tmpFile
fgrep "f1.c" $tmpFile
if [ "$?" != 0 ]; then
  let score-=6
  logMsg $LINENO "ERROR:  make did not rebuild f1.o after touching *.h"
fi

#Verify CFLAGS includes -Wall and c99
logMsg $LINENO "NOTE:  Grading use of gcc -Wall and c99"
result=$( cat Makefile )
deductMissingString $LINENO "$result" Wall 1
deductMissingString $LINENO "$result" c99 1

#Verify make clean works
logMsg $LINENO "NOTE:  Grading make clean"
make clean
deductFileExists $LINENO main.o 1
deductFileExists $LINENO f1.o 1
deductFileExists $LINENO f2.o 1
deductFileExists $LINENO f3.o 1
deductFileExists $LINENO myprog 1

#Verify make clean succeeds in a clean directory
logMsg $LINENO "NOTE:  Grading make clean in a clean directory"
rm -f *.o *.d myprog main                           #Manually clean this student's directory
make clean
if [ "$?" != 0 ]; then
  let score-=3
  logMsg $LIENNO "ERROR:  make clean in a clean directory failed"
fi

#Fix negative score after a wreck
if [ "$score" -lt 0 ]; then
  let score=0
fi

#Record the score and output to console
logMsg $LINENO "NOTE:  Finished grading" >>$dest
echo "Grade:  $score points" >>$dest
echo "Grade:  $score points"

