#!/bin/bash

WorkSpaceDir=$(dirname $(readlink -f "$0"))

rm -rf $WorkSpaceDir/src/comm/comm_msg/include_for_reading_only/comm_msg/*
if [ $? -ne 0 ] ; then
    echo "Del Old Head Files Fail."
    exit 1
fi

cp $WorkSpaceDir/devel/include/comm_msg/* $WorkSpaceDir/src/comm/comm_msg/include_for_reading_only/comm_msg
if [ $? -ne 0 ] ; then
    echo "Copy Head Files Fail."
    exit 1
fi

sync
exit 0
