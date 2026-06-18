#!/bin/bash

# 该脚本放在工程目录下
WorkSpaceDir=$(dirname $(readlink -f "$0"))

BuildDir=$WorkSpaceDir/build
cd $BuildDir
if [ $? -ne 0 ]; then
    echo "Can Not Find Dir $BuildDir"
else
    rm -rf ./*
    echo "Del All Files In $BuildDir"
fi

LibDir=$WorkSpaceDir/devel/lib
cd $LibDir
if [ $? -ne 0 ]; then
    echo "Can Not Find Dir $LibDir"
else
    rm -rf ./*
    echo "Del All Files In $LibDir"
fi

IncludeDir=$WorkSpaceDir/devel/include
cd $IncludeDir
if [ $? -ne 0 ]; then
    echo "Can Not Find Dir $IncludeDir"
else
    rm -rf ./*
    echo "Del All Files In $IncludeDir"
fi

ShareDir=$WorkSpaceDir/devel/share
cd $ShareDir
if [ $? -ne 0 ]; then
    echo "Can Not Find Dir $ShareDir"
else
    rm -rf ./*
    echo "Del All Files In $ShareDir"
fi

sync
exit 0




