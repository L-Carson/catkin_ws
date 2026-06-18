#!/bin/bash

CurWorkDir=$(pwd)

# Used for root, so the dir must be full path
# 该脚本放在工程目录下
WorkSpaceDir=$(dirname $(readlink -f "$0"))
cd $WorkSpaceDir
if [ $? -ne 0 ]; then
    echo "Cd Workspace Dir ($WorkSpaceDir) Fail."
    exit 1
fi

rm -rf $WorkSpaceDir/src/*

cd $CurWorkDir


sync
exit 0