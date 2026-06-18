#!/bin/bash

function EchoErrInfo()
{
    echo -e '\033[1;31m    Error: ' "$1" '\033[0m'
}

# 根据系统版本获取目标文件目录名
SystemVerInfo=`uname  -a`
if   [[ $SystemVerInfo =~ .*14.04.*Ubuntu.* ]] ; then
    ExecDirName="exec_x86_32"
    LibDirName="self_exe_so_x86_32"
elif [[ $SystemVerInfo =~ .*18.04.*Ubuntu.* ]] ; then
    ExecDirName="exec_x86_64_18.04"
    LibDirName="self_exe_so_x86_64_18.04"
elif [[ $SystemVerInfo =~ .*20.04.*Ubuntu.* ]] ; then
    ExecDirName="exec_x86_64_20.04"
    LibDirName="self_exe_so_x86_64_20.04"
elif [[ $SystemVerInfo =~ .*aarch64.GNU/Linux ]] ; then
    ExecDirName="exec_arm_64"
    LibDirName="self_exe_so_arm_64"
else
    EchoErrInfo "Unkown System!!!"
    exit 1
fi

# 该脚本放在工程目录下
WorkSpaceDir=$(dirname $(readlink -f "$0"))
GenFileRootDir=$WorkSpaceDir/devel/lib
ExeFileTgtDir=$(dirname $WorkSpaceDir)/exec/$ExecDirName
LibFileTgtDir=$(dirname $WorkSpaceDir)/self_exe_so/$LibDirName

#               包名                           生成文件                              是否车辆运行文件     是否建图软件运行文件     是否为Orin运行文件
GenFileList=('loc_node                       loc_play_node                                  flase                true                   false'             \
             'mapping_node                   mapping_node                                   flase                true                   false'             \
             'map_loc_api                    libmap_loc_api.so                              flase                true                   false'             \
             'simulation_perception          simulation_perception                          flase                true                   false'             \
             'simulation_location_motion     simulation_location_motion                     flase                true                   false'             \
             'nav_road_network_api           libroad_network_api.so                         flase                true                   false'             \
             'nav_road_network_api           libroad_network_make_api.so                    flase                true                   false'             \
             'nav_road_network_api           libroad_network_simulation_api.so              flase                true                   false'             \
             'localization_sys               loc_node                                       true                 true                   false'             \
             'localization_sys               pose_score_statistic_node                      true                 false                  false'             \
             'nav_node                       nav_node                                       true                 true                   false'             \
             'pcpt_node                      pcpt_node                                      true                 false                  false'             \
             'pcpt_cv_node                   pcpt_cv_node                                   true                 false                   true'             \
             'pcpt_net_lib                   libpcpt_net_lib.so                             false                false                   true'             \
             'pcpt_net_node                  pcpt_net_node                                  false                false                   true'             \
			 'pcpt_lidar_preproc             pcpt_lidar_preproc_node                        true                 false                  false'             \
             'sensor_imu                     sensor_imu_node                                true                 false                  false'             \
             'sensor_lidar                   sensor_lidar_node                              true                 false                  false'             \
             'livox_lidar                    livox_lidar_node                               true                 false                  false'             \
             'lslidar_driver                 lidar_leishen_node                             true                 false                  false'             \
             'sensor_radar                   sensor_radar_node                              true                 false                  false'             \
             'sensor_gps                     sensor_gps_node                                true                 false                  false'             \
             'sensor_tof                     sensor_tof_node                                true                 false                  false'             \
             'sensor_ultrasonic              sensor_ultrasonic_node                         true                 false                  false'             \
             'sensor_camera                  sensor_camera_node                             true                 false                  false'             \
             'dcu_node                       dcu_node                                       true                 false                  false'             \
             'tool_debug                     send_ctrl                                      true                 false                  false'             \
             'tool_smooth_cfg                smooth_cfg                                     true                 false                  false'             \
             'tool_calibration_chassis       calibration_steer_angle                        true                 false                  false'             \
             'tool_calibration_chassis       calibration_steer_response                     true                 false                  false'             \
             'maintain_node                  maintain_node                                  true                 false                  false'             \
             'tool_collect_image_data2       tool_collect_image_data_node2                  true                 false                  false'             \
             'vision_image_saver             vision_image_saver_node                        true                 false                  false'             \
            )

MoveFileList=()

function GetGenFileInfoByName()
{
    SearchGenFileName="$1"

    for  i  in "${GenFileList[@]}" ; do
        GenFileInfo=($i)
        GenFileName=${GenFileInfo[1]}

        if [ "$GenFileName" = "$SearchGenFileName" ] ; then
            echo "$i"
        fi
    done

    echo ""
}

function MoveGenFile()
{
    #echo "MoveGenFile Para1: "$1

    GenFileInfo=($1)
    if [ ${#GenFileInfo[*]} -lt 2 ] ; then
        EchoErrInfo "Move Gen File Info($1) Is Err."
        return 1
    fi

    PackageName=${GenFileInfo[0]}
    GenFileName=${GenFileInfo[1]}

    SrcFilePath=""
    TgtFileDir=""
    TgtFilePath=""
    if [[ $GenFileName == lib*.* ]] ; then
        SrcFilePath="$GenFileRootDir/$GenFileName"
        TgtFileDir="$LibFileTgtDir"
        TgtFilePath="$TgtFileDir/$GenFileName"
    else
        SrcFilePath="$GenFileRootDir/$PackageName/$GenFileName"
        TgtFileDir="$ExeFileTgtDir"
        TgtFilePath="$TgtFileDir/$GenFileName"
    fi

    #echo "SrcFilePath : $SrcFilePath"
    #echo "TgtFileDir  : $TgtFileDir"
    #echo "TgtFilePath : $TgtFilePath"

    if [ -e $SrcFilePath ]; then
        rm -rf $TgtFilePath
        if [ -e $TgtFilePath ]; then
            EchoErrInfo "Del File($TgtFilePath) Fail."
            return 1
        fi

        cp $SrcFilePath $TgtFileDir
        if [ ! -e $TgtFilePath ]; then
            EchoErrInfo "Copy File($SrcFilePath) Fail"
            return 1
        fi

        MoveFileList+=($GenFileName)
    fi

    return 0
}

function EchoMoveFileList()
{
    echo ''
    echo -e '\033[1;31m      Move ' "${#MoveFileList[*]} Files To $ExecDirName Or $LibDirName:" '\033[0m'
    for i in "${MoveFileList[@]}" ; do
        echo -e '\033[1;31m          '$i'\033[0m'
    done
    echo ''
}

function MoveGenFileByName()
{
    MoveGenFileName=$1
    MoveGenFileInfo=$(GetGenFileInfoByName "$MoveGenFileName")
    if [ "$MoveGenFileInfo" = "" ] ; then
        EchoErrInfo "Gen File($MoveGenFileName) Not Exist!!!"
        return 1
    fi

    MoveGenFile "$MoveGenFileInfo"
    if [ $? -ne 0 ] ; then
        EchoErrInfo "Move File($MoveGenFileInfo) Fail."
        return 1
    fi

    EchoMoveFileList
    return 0
}

function MoveAllExistGenFiles()
{
    IsMoveForCar=$1
    IsMoveForMapTool=$2
    IsMoveForPartOrin=$3

    for  i  in "${GenFileList[@]}" ; do
        GenFileInfo=($i)

        IsForCar=${GenFileInfo[2]}
        IsForMapTool=${GenFileInfo[3]}
		IsForPartOrin=${GenFileInfo[4]}

        if [ "$IsForCar" = "true" -a "$IsMoveForCar" = "true" ] || [ "$IsForMapTool" = "true" -a "$IsMoveForMapTool" = "true" ] || [ "$IsForPartOrin" = "true" -a "$IsMoveForPartOrin" = "true" ]; then
            MoveGenFile "$i"
            if [ $? -ne 0 ] ; then
                EchoErrInfo "Move File($i) Fail."
                return 1
            fi
        fi
    done

    EchoMoveFileList
    return 0
}
