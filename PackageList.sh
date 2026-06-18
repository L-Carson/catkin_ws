#!/bin/bash

#                 Package Name                  上车必须    建图必须  Orin  Gen Exe Or Lib 1                Gen Exe Or Lib 2                Gen Exe Or Lib 3                    Gen Exe Or Lib 4
Pack_Tgt_Map_Lst=("core_lib                        true     true    true"                                                                                                                                          \
                                                                                                                                                                                                                \
                  "comm_msg                        true     true    true"                                                                                                                                               \
                  "comm_srvs                       true     true    true"                                                                                                                                               \
                  "comm                            true     true    true      libcomm.a"                                                                                                                                  \
                  "comm_extend                     true     true    true      libcomm_extend.a"                                                                                                                           \
                  "comm_ros_extend                 true     true    true      libcomm_ros_extend.a"                                                                                                                       \
                                                                                                                                                                                                                \
                  "maintain_node                   true     false   false     maintain_node"                                                                                                                              \
                                                                                                                                                                                                                \
                  "comm_layer                      true     false   false     libcomm_layer.a"                                                                                                                            \
                  "base_layer                      true     false   false     libbase_layer.a"                                                                                                                            \
                  "control_layer                   true     false   false     libcontrol_layer.a"                                                                                                                         \
                  "plan_layer                      true     false   false     libplan_layer.a"                                                                                                                            \
                  "simulation_layer                false    false   false"                                                                                                                                              \
                                                                                                                                                                                                                \
                  "nav_comm                        true     true    false     libnav_comm.a"                                                                                                                              \
                  "nav_algorithm                   true     true    false     libnav_algorithm.a"                                                                                                                         \
                  "nav_linear_vel_plan             true     true    false     libnav_linear_vel_plan.a"                                                                                                                   \
                  "nav_along_route_ctrl            true     true    false     libnav_along_route_ctrl.a"                                                                                                                  \
                  "nav_hybrid_astar                true     true    false     libnav_hybrid_astar.a"                                                                                                                      \
                  "nav_multi_trj_sw_plan           true     true    false     libnav_multi_trj_sw_plan.a"  
                  "nav_escape                      true     true    false     libnav_escape.a"                                                                                                                            \                                                                                                               \
                  "nav_fixed_route                 true     true    false     libnav_fixed_route.a"                                                                                                                       \
                  "nav_open_space                  true     true    false     libnav_open_space.a"                                                                                                                        \
                  "nav_comm_core                   true     true    false     libnav_comm_core.a"                                                                                                                         \
                  "nav_node                        true     true    false     nav_node"                                                                                                                                   \
                  "nav_road_network_api            false    true    false     libroad_network_api.so          libroad_network_make_api.so     libroad_network_simulation_api.so   libroad_network_simulation_hmi.a"       \
                                                                                                                                                                                                                \
                  "loc_comm                        true     true    false     libloc_comm.a"                                                                                                                              \
                  "loc_node                        true     true    false     loc_node"                                                                                                                                   \
                  "mnl_utility                     true     true    false     libmnl_utility.a"                                                                                                                           \
                  "mnl_core                        true     true    false     libmnl_core.a"                                                                                                                              \
                  "mnl_msgs                        true     true    false"                                                                                                                                               \
                  "mnl_ros                         true     true    false     libmnl_ros.a"                                                                                                                               \
                  "graph_optimizer                 false    true    false     libgraph_optimizer.a"                                                                                                                       \
                  "map_manager                     true     true    false     libmap_manager.a"                                                                                                                           \
                  "mapping_sys                     false    true    false     libmapping_sys_core.a           libmapping_sys.so"                                                                                          \
                  "localization_sys                true     false   false     liblocalization_core.a          loc_node                        pose_score_statistic_node"                                                  \
                                                                                                                                                                                                                \
                  "pcpt_comm                       true     false   true      libpcpt_comm.a"                                                                                                                             \
                  "pcpt_lidar_preproc              true     false   false     pcpt_lidar_preproc_node"                                                                                                                    \
                  "pcpt_layer                      true     false   false     libpcpt_layer.a"                                                                                                                            \
                  "pcpt_data                       true     false   false     libpcpt_data.a"                                                                                                                             \
			      "pcpt_peripheral                 true     false   false     libpcpt_peripheral.a"                                                                                                                       \	  
                  "pcpt_det_dynamic                true     false   false     libpcpt_det_dynamic.a"                                                                                                                      \
                  "pcpt_det_static                 true     false   false     libpcpt_det_static.a"                                                                                                                       \
                  "pcpt_node                       true     false   false     pcpt_node"                                                                                                                                  \
                  "pcpt_cv_node                    true     false   true      pcpt_cv_node"                                                                                                                               \
                  "pcpt_sim_node                   false    false   false     pcpt_sim_node"                                                                                                                              \
                  "pcpt_daq_node                   false    false   false     libdraco.a"                                                                                                                                 \
                  "pcpt_roadmark_node              false    false   false     pcpt_roadmark_node"                                                                                                                         \
                  "pcpt_mapping_node               false    true    false     pcpt_mapping_node"                                                                                                                          \
                  "pcpt_fast_local_map             false    false   false     pcpt_fast_local_map"                                                                                                                        \
                  "pcpt_net_lib                    false    false   true      libpcpt_net_lib.so"                                                                                                                         \
                  "pcpt_net_node                   false    false   true      pcpt_net_node"                                                                                                                              \
                  "pcpt_det3d_node                 false    false   false     pcpt_det3d_node"                                                                                                                            \
                                                                                                                                                                                                                \
                  "sensor_imu                      true     false   false     sensor_imu_node"                                                                                                                            \
                  "comm_lidar_3d                   true     false   false     libcomm_lidar_3d.a"                                                                                                                         \
                  "sensor_lidar                    true     false   false     sensor_lidar_node"                                                                                                                          \
                  "livox_lidar                     true     false   false     livox_lidar_node"                                                                                                                           \
                  "lslidar_msgs                    true     false   false"                                                                                                                                              \
                  "lslidar_driver                  true     false   false     lidar_leishen_node"                                                                                                                         \
                  "sensor_radar                    true     false   false     sensor_radar_node"                                                                                                                          \
                  "sensor_gps                      true     false   false     sensor_gps_node"                                                                                                                            \
                  "sensor_tof                      true     false   false     sensor_tof_node"                                                                                                                            \
                  "sensor_ultrasonic               true     false   false     sensor_ultrasonic_node"                                                                                                                     \
                  "sensor_camera                   true     false   false     sensor_camera_node"                                                                                                                         \
                                                                                                                                                                                                                \
                  "dcu_node                        true     false   false     dcu_node"                                                                                                                                   \
                                                                                                                                                                                                                \
                  "simulation_dcu                  false    false   false     simulation_dcu"                                                                                                                             \
                  "simulation_pcb                  false    false   false     simulation_pcb"                                                                                                                             \
                  "simulation_hmi                  false    false   false     simulation_hmi"                                                                                                                             \
                  "simulation_perception           false    true    false     simulation_perception"                                                                                                                      \
                  "simulation_location_motion      false    true    false     simulation_location_motion"                                                                                                                 \
                                                                                                                                                                                                                \
                  "tool_debug                      true     false   false     send_ctrl"                                                                                                                                  \
                  "tool_make_nav_map               false    false   false     show_pcloud_map                 show_road_network               gen_line"                                                                   \
                  "tool_calibration                false    false   false     calibrate_turn_angle_node       calibrate_wheel_speed_node"                                                                                 \
                  "tool_calibration_camera         false    false   false     tool_calibration_camera_node"                                                                                                               \
                  "tool_statis_info                false    false   false     show_statis_info_node"                                                                                                                      \
                  "tool_pack                       false    false   false     upt_pack_description_file"                                                                                                                  \
                  "tool_smooth_cfg                 true     false   false     smooth_cfg"                                                                                                                                 \
                  "tool_process_data               false    false   false     show_info_on_images"                                                                                                                        \
                  "tool_calibration_chassis        true     false   false     calibration_steer_angle         calibration_steer_response" 
                  "tool_collect_image_data2        true     false   false     tool_collect_image_data_node2"                                                                                              \
                  "vision_image_saver              true     false   false     vision_image_saver_node"                                                                                                                    \
                  )

# function CheckPackExist()
# {
#     Pack=$1
#     ModulePathList=(`ls -d ./src/*/*/ 2>/dev/null`)
#     #echo "${ModulePathList[*]}"

#     for ModulePath in "${ModulePathList[@]}" ; do
#         if [ -d "$ModulePath$Pack" ] ; then
#             return 0
#         fi
#     done

#     return 1
# }

function CheckPackExist()
{
    local Pack=$1
    local Found=1  # 默认未找到（1表示失败）
    
    # 查找所有包含 CMakeLists.txt 的目录（ROS功能包标志）
    while IFS= read -r -d '' ModulePath; do
        # 获取功能包名称（目录名）
        local PackName=$(basename "$ModulePath")
        
        # 如果匹配目标包名
        if [ "$PackName" == "$Pack" ]; then
            Found=0  # 找到
            break
        fi
    done < <(find ./src -type f -name "CMakeLists.txt" -printf "%h\0" 2>/dev/null | sort -uz)
    
    return $Found
}

function GetPackNameByBuildInfo()
{
    PackBuildInfo=$1
    if [ "$PackBuildInfo" = "" ] ; then
        echo ""
    else
        PackBuildInfoItemList=($PackBuildInfo)
        echo ${PackBuildInfoItemList[0]}
    fi
}

function GetPackGenFilePathListStrByBuildInfo()
{
    PackBuildInfo=$1

    PackName=$(GetPackNameByBuildInfo "$PackBuildInfo")
    PackBuildInfoItemList=($PackBuildInfo)

    PackGenFilePathLst=()
    for index in ${!PackBuildInfoItemList[@]} ; do
        if [ $index -gt 3 ] ; then
            PackGenFileName=${PackBuildInfoItemList[$index]}
            if [[ $PackGenFileName == lib*.* ]] ; then
                PackGenFilePath="./devel/lib/$PackGenFileName"
            else
                PackGenFilePath="./devel/lib/$PackName/$PackGenFileName"
            fi

            PackGenFilePathLst[$(($index-1))]=$PackGenFilePath
        fi
    done

    echo "${PackGenFilePathLst[*]}"
}

# 获取所有包列表，可以输出关键字进行过滤
function GetAllPackList()
{
    Pack_List=()
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -lt 1 ] ; then
            continue
        fi

        Pack=${Pack_Tgt_Map[0]}
        if [ "$1" != "" ] ; then
            result=$(echo "$Pack" | grep "$1")
            if [[ "$result" == "" ]] ; then
                continue
            fi
        fi

        Pack_List+=($Pack)
    done

    echo "${Pack_List[*]}"
}

# 获取当前存在的包列表，可以输入关键字进行过滤
function GetExistPackList()
{
    Pack_List=()
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -lt 1 ] ; then
            continue
        fi

        Pack=${Pack_Tgt_Map[0]}

        if [ "$1" != "" ] ; then
            result=$(echo "$Pack" | grep "$1")
            if [[ "$result" == "" ]] ; then
                continue
            fi
        fi

        CheckPackExist $Pack
        if [ $? -eq 0 ] ; then
            Pack_List+=($Pack)
        fi
    done

    echo "${Pack_List[*]}"
}

function GetPackListForCar()
{
    Pack_List=()
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -lt 2 ] ; then
            continue
        fi

        Pack=${Pack_Tgt_Map[0]}
        if [ "$1" != "" ] ; then
            result=$(echo "$Pack" | grep "$1")
            if [[ "$result" == "" ]] ; then
                continue
            fi
        fi

        IsNeedForCar=${Pack_Tgt_Map[1]}
        if [ "$IsNeedForCar" != "true" ] ; then
            continue
        fi

        Pack_List+=($Pack)
    done

    echo "${Pack_List[*]}"
}

function GetPackListForMapTool()
{
    Pack_List=()
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -lt 3 ] ; then
            continue
        fi

        Pack=${Pack_Tgt_Map[0]}
        if [ "$1" != "" ] ; then
            result=$(echo "$Pack" | grep "$1")
            if [[ "$result" == "" ]] ; then
                continue
            fi
        fi

        IsNeedForMapTool=${Pack_Tgt_Map[2]}
        if [ "$IsNeedForMapTool" != "true" ] ; then
            continue
        fi

        Pack_List+=($Pack)
    done

    echo "${Pack_List[*]}"
}

function GetPackListForPartOrin()
{
    Pack_List=()
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -lt 3 ] ; then
            continue
        fi

        Pack=${Pack_Tgt_Map[0]}
        if [ "$1" != "" ] ; then
            result=$(echo "$Pack" | grep "$1")
            if [[ "$result" == "" ]] ; then
                continue
            fi
        fi

        IsNeedForPartOrin=${Pack_Tgt_Map[3]}
        if [ "$IsNeedForPartOrin" != "true" ] ; then
            continue
        fi

        Pack_List+=($Pack)
    done

    echo "${Pack_List[*]}"
}

function GetBuildPackInfo()
{
    for  i  in "${Pack_Tgt_Map_Lst[@]}" ; do
        Pack_Tgt_Map=($i)
        if [ ${#Pack_Tgt_Map[@]} -gt 0 ] ; then
            if [ ${Pack_Tgt_Map[0]} = "$1" ] ; then
                echo "$i"
            fi
        fi
    done

    echo ""
}

function DelExistPacksExcept()
{
    exceptPackLst=($1)
    #echo "exceptPackLst: $1"

    existPackLstStr=$(GetExistPackList)
    existPackLst=($existPackLstStr)
    #echo "existPackLst: $existPackLstStr"

    for existPack in "${existPackLst[@]}" ; do
        isPackExcept=0

        for exceptPack in "${exceptPackLst[@]}" ; do
            if [ "$exceptPack" == "$existPack" ] ; then
                isPackExcept=1
                break
            fi
        done

        if [ $isPackExcept -eq 0 ] ; then
            rm -rf ./src/*/$existPack
            echo "Del Package $existPack"
        fi
    done
}

function EchoInfo()
{
    echo "####"
    echo -e "####\033[1;31m      $1\033[0m"
    echo "####"
}

function EchoPackList()
{
    if [ "$1" = "" ] ; then
        echo "Please Input Key Info!!!!!"
        exit
    fi

    if [ "$2" = "" ] ; then
        echo "Please Input Pack List!!!!!"
        exit
    fi

    packList=($2)

    echo "####"
    echo -e "####\033[1;31m      $1  (Num = ${#packList[*]})\033[0m"

    for i in "${packList[@]}" ; do
        echo -e "####\033[1;31m          $i\033[0m"
    done

    echo "####"
}
