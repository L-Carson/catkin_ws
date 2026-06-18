#!/bin/bash

source ./PackageGenFileList.sh

if [ "$1" = "" ] ; then
    # 未指定时，移动所有存在的执行文件
    MoveAllExistGenFiles "false" "true" "false"
    if [ $? -ne 0 ] ; then
        exit 1
    fi
    
else
    # 指定执行文件时，只移动指定执行文件
    MoveGenFileByName "$1"
    if [ $? -ne 0 ] ; then
        exit 1
    fi

fi

sync
exit 0

