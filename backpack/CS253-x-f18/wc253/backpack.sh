#!/bin/bash
#-------------------------------------------------------------------------------
# NAME
#   backpackpc.sh -- Grades the word-count project awarding partial credit
#
# SYNOPSIS
#   backpack.sh file
#
# DESCRIPTION
#   The backpackpc.sh grades the CS253 wc253 project, awarding partial
#   credit.  The script keeps a running total of a students score, and, after
#   a problem is encountered, attempts to grade whatever else might work.  
#
#   The script records feedback to the student in the specified file in a
#   format that can also be processed by the "backpack -g" auto-grader command.
#
#   Most messages written by the script include the line-number of their origin:
#     [42] Foo Fo Fi Fum Invalid Englishman found
#   in brackets, [42].  If you open the script in an editor, you can examine
#   around the line of their origin to investigate what is happening there.
#
# HISTORY
#   02/03/2018 Epoch..........................................................jrc
#   09/21/2018 Change scoring to deduct rather than accumulate points.........jrc
#--------------------------------------------------------------------------------
if [ "$1" = "" ]; then
  echo "usage: $0 outputfile"
  exit 0;
fi

#Configuration that can be easily modified
let maxscore=50                  #The maximum score allotted for this assignment
errMsg=ErrorsAndWarnings.txt
EXE=wc253


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
# Grading begins here.  Remove artifacts from previous grading in this directory
#---------------------------------------------------------------------------------
rm -f $dest                                         #Remove old grading artifacts
logMsg $LINENO "NOTE:  Begin grading on `date`"
logMsg $LINENO "NOTE:  Using temp file, $tmpFile"

#Verify the expected Makefile name
logMsg $LINENO "NOTE:  Grading expected Makefile name"
deductFileMissing $LINENO Makefile 3

#Manually clean old junk from previous builds and grading attempts
rm -f *.o *.d expected actual $errMsg *.result

#Verify make will build the default target
logMsg $LINENO "NOTE:  Grading make default target"
make
if [ "$?" -gt 0 ] ; then
  deductPoints 10
logMsg $LINENO "ERROR:  Make exited abnormally building the default target"
fi

#Make sure the program built with the expected name
logMsg $LINENO "NOTE:  Grading expected executable program, $EXE, exists"
deductFileMissing $LINENO $EXE 50

#Verify exit status given an empty dataset----------------------------------------
logMsg $LINENO "Note:  Grading exit status given an empty dataset, /dev/null"
./$EXE /dev/null >>$errMsg 2>&1
if [ "$?" != 0 ]; then
  logMsg $LINENO "ERROR: $EXE did not run/exit normally"
  deductPoints 5
fi

#Verify wc253 -l data exit status-------------------------------------------------
logMsg $LINENO "Note:  Grading $EXE -l data"
./$EXE -l data >>$errMsg 2>&1
if [ "$?" != 0 ]; then
  logMsg $LINENO "ERROR: $EXE -l data exited abnormally"
  deductPoints 5
fi

#Verify program rejects an invalid option (-x)------------------------------------
logMsg $LINENO "Note:  Grading invalid -x option"
./$EXE -x data >>$errMsg 2>&1
if [ "$?" == 0 ]; then
  logMsg $LINENO "ERROR: $EXE -x data exited with 0 status --- should reject -x"
  deductPoints 5
fi

#Run the student program on data-------------------------------------------------
logMsg $LINENO "Note:  Grading output from $EXE data"
./$EXE data     >data.result 2>>$errMsg
deductnGrep $LINENO  "769\s"     data.result 2
deductnGrep $LINENO  "\s5770\s"  data.result 2
deductnGrep $LINENO  "\s34553\s" data.result 2
deductnGrep $LINENO  "\sdata"    data.result 2

#Run the students program on data with -l----------------------------------------
logMsg $LINENO "Node: Grading file, data with -l option"
./$EXE -l data >datal.result 2>>$errMsg
deductnGrep $LINENO "769\sdata"  datal.result 2

#Run and grade the students program on data1-------------------------------------
logMsg $LINENO "Note:  Grading file, data1"
./$EXE data1   >data1.result 2>>$errMsg
deductnGrep $LINENO  "4\s"       data1.result 2
deductnGrep $LINENO  "\s30\s"    data1.result 2
deductnGrep $LINENO  "\s164\s"   data1.result 2
deductnGrep $LINENO "\sdata1"    data1.result 2

#Run the students program on data2 with -l---------------------------------------
logMsg $LINENO "Node: Grading file, data1 with -l"
./$EXE -l data1 > datal1.result 2>>$errMsg
deductnGrep $LINENO "4\sdata1"   datal1.result 2

#Run and grade the students program on data2-------------------------------------
logMsg $LINENO "Note:  Grading file, data2"
./$EXE data2   >data2.result 2>>$errMsg
deductnGrep $LINENO  "9\s"       data2.result 2
deductnGrep $LINENO  "\s25\s"    data2.result 2
deductnGrep $LINENO  "\s171\s"   data2.result 2
deductnGrep $LINENO  "\sdata2"   data2.result 2

#Run and grade the students program on data2 with -l------------------------------
logMsg $LINENO "Note: Grading file, data2 with -l"
./$EXE -l data2 > datal2.result 2>>$errMsg
deductnGrep $LINENO "9\sdata2"   datal2.result 2

#Grade make clean-----------------------------------------------------------------
logMsg $LINENO "Note:  Grading make clean"
make clean
if [ "$?" != 0 ]; then
  logMsg $LINENO "ERROR: make clean did not exit normally"
  deductPoints 5
fi
deductFileExists $LINENO wc253 5

#Record the score and output to console--------------------------------------------
logMsg $LINENO "NOTE:  Finished grading" >>$dest
echo "Grade:  $score points" >>$dest
echo "Grading finished.  Results are in file, $dest"

