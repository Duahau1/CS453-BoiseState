#!/bin/bash
if [ ! -e INSTALL_COMPLETE ]
then
    echo "Did not find file INSTALL_COMPLETE"
    echo "Did you create the file in the correct directory?"
    echo "Use the ls command to verify that the file INSTALL_COMPLETE is present"
    echo "and try again"
    exit 1
fi

if [ -n "$(git status --porcelain)" ]; then
    echo "Make sure you have done the following"
    echo "   touch INSTALL_COMPLETE"
    echo "   git add INSTALL_COMPLETE"
    echo "   git commit -m <your message>"
    echo "   git push "
    echo ""
    echo "If you still see this message and you have completed all the steps"
    echo "above you are OK and have done the assignment. You will learn some"
    echo "more advanced steps in future modules"
    exit 0
fi

echo "Everything appears to be OK, good Job on your first offical git assignment!"
