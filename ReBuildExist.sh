#!/bin/bash

source ./PackageList.sh

if [ "$1" = "" ] ; then
    packLstStr=$(GetExistPackList)
else
    packLstStr=$(GetExistPackList "$1")
fi

if [ "$packLstStr" = "" ] ; then
    echo "Can Not Find Pack!!!!!"
    exit 1
fi

./ReBuild.sh "$packLstStr"
if [ $? -ne 0 ] ; then
    exit 1
fi

exit 0
