#!/bin/bash

source ./PackageList.sh

if [ "$1" = "" ] ; then
    echo "Not Input Begin Package!!!!"
    exit 1
fi

allPackLstStr=$(GetExistPackList)

allPackList=($allPackLstStr)
filteredPackList=()
for Pack in "${allPackList[@]}" ; do
    if [ ${#filteredPackList[@]} -gt 0 ] ; then
        filteredPackList+=($Pack)
        continue
    fi
        
    result=$(echo "$Pack" | grep "$1")
    if [[ ! "$result" == "" ]] ; then
        filteredPackList+=($Pack)
        continue
    fi
done
    
filteredPackListStr="${filteredPackList[*]}"
if [ "$filteredPackListStr" = "" ] ; then
    echo "Can Not Find Pack!!!!!"
    exit 1
fi

./Build.sh "$filteredPackListStr"
if [ $? -ne 0 ] ; then
    exit 1
fi

exit 0
