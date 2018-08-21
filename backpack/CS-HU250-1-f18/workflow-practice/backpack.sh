#!/bin/bash

if [ ! -e 0001-Workflow-practice.patch ]
then
    echo "FAIL: did no find the correct patch file"
    exit 1
else
    echo "Found the patch file"
fi

if [ -n "$(git status --porcelain)" ]; then
    echo "FAIL: You need to add, commit, and then push your changes";
    exit 1
fi

#everything appears to be OK
echo "Don't forget to run git push!"
