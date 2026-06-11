/*****************************************************************************
*  @brief                                                                    *
*  @author   Zenghui Luo                                                     *
*  @version  1.0                                                             *
*  @date     2022/04/15                                                      *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2022/04/15 | 1.0       | Zenghui Luo    | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef __COMMON_DEPEND__COMM_URDF_API_H__
#define __COMMON_DEPEND__COMM_URDF_API_H__

#include <stdio.h> 
#include <cstring>
#include <iostream>
#include <string>
#include <pwd.h>
#include <unistd.h>
#include <boost/array.hpp>

#include "comm/loc_xml/tinyxml.h"

/**
 * @brief URDF操作接口类
 * 
 */
class UrdfOptApiC
{
public:
    // 构造函数
    UrdfOptApiC(void);

    // 析构函数
    ~UrdfOptApiC(void);

    // 读取指定joint的origin
    int ReadGivenJointOrigin(const std::string &urdf_file,
                             const std::string &parent_frame_name,
                             const std::string &child_frame_name,
                             boost::array<float, 3> &rpy_value,
                             boost::array<float, 3> &xyz_value);

    // 写入指定joint的origin
    int WriteGivenJointOrigin(const std::string &urdf_file,
                              const std::string &parent_frame_name,
                              const std::string &child_frame_name,
                              const boost::array<float, 3> &rpy_value,
                              const boost::array<float, 3> &xyz_value,
                              const boost::array<bool, 3> &rpy_upt = {true, true, true},
                              const boost::array<bool, 3> &xyz_upt = {true, true, true});

private:
    // 搜索指定的joint
    bool SearchGivenJoint(TiXmlElement *p_root,
                          TiXmlElement **pp_joint,
                          const std::string &parent_frame_name,
                          const std::string &child_frame_name);

    // 是否为指定的joint
    bool WhetherGivenJoint(TiXmlElement *p_joint,
                           const std::string &parent_frame_name,
                           const std::string &child_frame_name);

    // 解析给定joint的origin
    bool ParsingGivenJointOrigin(TiXmlElement *p_joint,
                                 boost::array<float, 3> &rpy_value,
                                 boost::array<float, 3> &xyz_value);

    // 更新指定joint的origin
    bool UpdateGivenJointOrigin(TiXmlElement *p_joint,
                                const boost::array<float, 3> &rpy_value,
                                const boost::array<float, 3> &xyz_value,
                                const boost::array<bool, 3> &rpy_upt,
                                const boost::array<bool, 3> &xyz_upt);

    // 读取3D矢量属性
    bool ParsingVector3fAttribute(TiXmlElement *p_origin,
                                  const std::string &attribute_name,
                                  boost::array<float, 3> &data_buf);

    // 更新3D矢量属性
    void UpdateVector3fAttribute(TiXmlElement *p_origin,
                                 const std::string &attribute_name,
                                 const boost::array<float, 3> &new_value,
                                 const boost::array<bool, 3> &upt_buf);
};



#endif
