#!/bin/bash

# 该脚本放在工程目录下
WorkSpaceDir=$(dirname $(readlink -f "$0"))
cd $WorkSpaceDir
if [ $? -ne 0 ]; then
    echo "Cd Workspace Dir ($WorkSpaceDir) Fail."
    exit 1
fi

source ./PackageList.sh

function DelPackBuildDir()
{
    PackName=$1
    PackBuildDirs=(`ls -d ./build/*/$PackName 2>/dev/null`)

    for PackBuildDir in "${PackBuildDirs[@]}" ; do
        if [ -d "$PackBuildDir" ] ;  then
            rm -rf "$PackBuildDir"
            EchoInfo "Del Build Dir: $PackBuildDir"
        else
            EchoInfo "No Build Dir: $PackBuildDir"
        fi
    done
}

function DelPackGenFiles()
{
    PackGenFilePathList=($1)

    for PackGenFilePath in "${PackGenFilePathList[@]}" ; do
        if [ -f "$PackGenFilePath" ] ;  then
            rm -rf "$PackGenFilePath"
            EchoInfo "Del Gen File: $PackGenFilePath"
        else
            EchoInfo "No Gen File: $PackGenFilePath"
        fi
    done
}

function CheckPackGenFiles()
{
    PackGenFilePathList=($1)

    for PackGenFilePath in "${PackGenFilePathList[@]}" ; do
        if [ ! -f "$PackGenFilePath" ] ; then
            EchoInfo "Gen File< $PackGenFilePath > File Not Exist !!!!"
            return 1
        else
            EchoInfo "Gen File< $PackGenFilePath > File Exist !!!!"
        fi
    done

    return 0
}

function BuildPack()
{
    PackBuildInfo="$1"
    IsReBuild="$2"

    PackName=$(GetPackNameByBuildInfo "$PackBuildInfo")
    if [ "$PackName" = "" ] ; then
        EchoInfo "Package Build Info Is Null !!!!"
        return 1
    fi

    EchoInfo "Begin To Build Package < $PackName > ."

    # 编译准备
    find ./src/ -name "*.cfg" -exec chmod 777 {} \;

    CheckPackExist "$PackName"
    if [ $? -ne 0 ] ; then
        EchoInfo "Package< $PackName > Not Exist !!!!"
        return 1
    fi
    
    # 如果是重编译，则删除编译中间件；否则增量编译。
    if [ "$IsReBuild" = "TRUE" ] ; then
        PackGenFilePathListStr=$(GetPackGenFilePathListStrByBuildInfo "$PackBuildInfo")
        DelPackBuildDir "$PackName"
        DelPackGenFiles "$PackGenFilePathListStr"
    fi

    # 编译
    catkin_make -DCATKIN_WHITELIST_PACKAGES=$PackName

    # 检查编译结果
    CheckPackGenFiles "$PackGenFilePathListStr"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    EchoInfo "Build Package < $PackName > Succ."
    return 0
}

function BuildPackLst()
{
    Build_Pack_List=($1)
    IsReBuild="$2"
    
    for Pack in "${Build_Pack_List[@]}" ; do
        BuildPackInfo=$(GetBuildPackInfo "$Pack")
        if [ "$BuildPackInfo" = "" ] ; then
            EchoInfo "$Pack Pack Not Exist!!!!"
            return 1
        fi


        BuildPack "$BuildPackInfo" "$IsReBuild"
        if [ $? -ne 0 ] ; then
            EchoInfo "Build Package < $PackName > Fail."
            return 1
        fi
    done

    EchoPackList "Build All Package Succ !!!" "${Build_Pack_List[*]}"
    return 0
}

function EchoTimeLen()
{
    Start_Build_Time=$1
    Start_Build_Time_Sec=$(date --date="$Start_Build_Time" +%s);

    End_Build_Time=$2
    End_Build_Time_Sec=$(date --date="$End_Build_Time" +%s);

    Build_Time_Len=$((End_Build_Time_Sec-Start_Build_Time_Sec))
    Build_Time_Len_H=$((Build_Time_Len/3600))
    Build_Time_Len_M=$(((Build_Time_Len-Build_Time_Len_H*3600)/60))
    Build_Time_Len_S=$(( Build_Time_Len-Build_Time_Len_H*3600-Build_Time_Len_M*60))

    EchoInfo "编译总时长： "${Build_Time_Len_H}"小时"${Build_Time_Len_M}"分"${Build_Time_Len_S}"秒 ("${Build_Time_Len}"秒)"
}

EchoInfo ""
EchoInfo ""

BuildPackList="$1"
IsReBuild="$2"

if [ "$BuildPackList" = "" ] ; then
    echo "Please Input Pack List After Cmd!!!!!"
    exit 1
fi

if [ ! $IsReBuild == TRUE ] && [ ! $IsReBuild == "" ]; then
    echo "Input Rebuild Sw Error: $IsReBuild"
    exit 1
else
    EchoInfo "Is Rebuild: $IsReBuild"
fi

EchoPackList "Pack List :" "$BuildPackList"

# 记录开始编译时间
Start_Build_Time=`date +'%Y-%m-%d %H:%M:%S'`

# 开始编译
BuildPackLst "$BuildPackList" "$IsReBuild"
BuildRslt=$?

# 输出编译花费时间
End_Build_Time=`date +'%Y-%m-%d %H:%M:%S'`
EchoTimeLen "$Start_Build_Time" "$End_Build_Time"

if [ $BuildRslt -ne 0 ] ; then
    exit 1
fi

exit 0
