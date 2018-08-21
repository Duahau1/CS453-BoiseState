#!/bin/bash
git checkout master
git checkout patch-practice
if [ ! $? -eq 0 ]
then
    echo "FAIL: Could not checkout the patch-practice branch"
    exit 1
fi

pushd  ../../../250/projects/patch-practice

if [ ! -e HelloWorld.java  ]
then
    echo "FAIL: Did not find file HelloWorld.java"
    exit 1
fi

if [ -n "$(git status --porcelain)" ]; then
    echo "FAIL: Git status is not clean"
    exit 1
fi

popd

git checkout master

branch="$(git branch -r)"
if [[ $branch =~ origin/patch-practice ]]
then
    echo "Found remote branch"
else
    echo "Did NOT find a remote branch"
fi
