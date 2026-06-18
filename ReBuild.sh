#!/bin/bash


BuildPackList="$1"

if [ "$BuildPackList" = "" ] ; then
    echo "Please Input Pack After Cmd!!!!!"
    exit 1
fi

./Build.sh "$BuildPackList" TRUE
if [ $? -ne 0 ] ; then
    exit 1
fi

exit 0
