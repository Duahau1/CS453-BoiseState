#!/bin/bash
if [ ! -e INSTALL_COMPLETE ]
then
    echo "FAIL: Did not find file INSTALL_COMPLETE"
    exit 1
fi

if [ -n "$(git status --porcelain)" ]; then
    echo "FAIL: You need to add, commit, and then push your changes";
    exit 1
fi

#everything appears to be OK
echo "Don't forget to run git push!"
