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
#include "comm/comm_urdf_api.h"

// 构造函数
UrdfOptApiC::UrdfOptApiC(void)
{
}

// 析构函数
UrdfOptApiC::~UrdfOptApiC(void)
{
}

// 读取指定joint的origin
int UrdfOptApiC::ReadGivenJointOrigin(const std::string &urdf_file,
                                      const std::string &parent_frame_name,
                                      const std::string &child_frame_name,
                                      boost::array<float, 3> &rpy_value,
                                      boost::array<float, 3> &xyz_value)
{
    TiXmlDocument xml_doc(urdf_file);
    if (!xml_doc.LoadFile())
    {
        return -1;
    }

    TiXmlElement *p_root = xml_doc.RootElement();
    if (nullptr == p_root)
    {
        return -2;
    }

    TiXmlElement *p_joint = nullptr;
    if (!SearchGivenJoint(p_root, &p_joint, parent_frame_name, child_frame_name))
    {
        return -3;
    }

    if (!ParsingGivenJointOrigin(p_joint, rpy_value, xyz_value))
    {
        return -4;
    }

    return 0;
}

// 写入指定joint的origin
int UrdfOptApiC::WriteGivenJointOrigin(const std::string &urdf_file,
                                       const std::string &parent_frame_name,
                                       const std::string &child_frame_name,
                                       const boost::array<float, 3> &rpy_value,
                                       const boost::array<float, 3> &xyz_value,
                                       const boost::array<bool, 3> &rpy_upt,
                                       const boost::array<bool, 3> &xyz_upt)
{
    TiXmlDocument xml_doc(urdf_file);
    if (!xml_doc.LoadFile())
    {
        return -1;
    }

    TiXmlElement *p_root = xml_doc.RootElement();
    if (nullptr == p_root)
    {
        return -2;
    }

    TiXmlElement *p_joint = nullptr;
    if (!SearchGivenJoint(p_root, &p_joint, parent_frame_name, child_frame_name))
    {
        return -3;
    }

    if (!UpdateGivenJointOrigin(p_joint, rpy_value, xyz_value, rpy_upt, xyz_upt))
    {
        return -4;
    }

    // 保存文件
    xml_doc.SaveFile();
    return 0;
}

// 搜索指定的joint
bool UrdfOptApiC::SearchGivenJoint(TiXmlElement *p_root,
                                   TiXmlElement **pp_joint,
                                   const std::string &parent_frame_name,
                                   const std::string &child_frame_name)
{
    if (nullptr == p_root)
    {
        return false;
    }

    for (TiXmlElement *p_child = p_root->FirstChildElement();
         nullptr != p_child; p_child = p_child->NextSiblingElement())
    {
        if ((strcmp("joint", p_child->Value()) != 0))
        {
            continue;
        }

        if (!WhetherGivenJoint(p_child, parent_frame_name, child_frame_name))
        {
            continue;
        }

        std::string joint_name = p_child->Attribute("name");
        if (nullptr != pp_joint)
        {
            *pp_joint = p_child;
        }
        return true;
    }
    return false;
}

// 是否为指定的joint
bool UrdfOptApiC::WhetherGivenJoint(TiXmlElement *p_joint,
                                    const std::string &parent_frame_name,
                                    const std::string &child_frame_name)
{
    TiXmlElement *p_child = p_joint->FirstChildElement("child");
    if (nullptr == p_child)
    {
        return false;
    }

    std::string child_link = p_child->Attribute("link");
    if (child_frame_name != child_link)
    {
        return false;
    }

    TiXmlElement *p_parent = p_joint->FirstChildElement("parent");
    if (nullptr == p_parent)
    {
        return false;
    }

    std::string parent_link = p_parent->Attribute("link");
    if (parent_frame_name != parent_link)
    {
        return false;
    }

    return true;
}

// 解析给定joint的origin
bool UrdfOptApiC::ParsingGivenJointOrigin(TiXmlElement *p_joint,
                                          boost::array<float, 3> &rpy_value,
                                          boost::array<float, 3> &xyz_value)
{
    if (nullptr == p_joint)
    {
        return false;
    }

    for (TiXmlElement *p_child = p_joint->FirstChildElement();
         nullptr != p_child; p_child = p_child->NextSiblingElement())
    {
        if ((strcmp("origin", p_child->Value()) != 0))
        {
            continue;
        }

        bool rpy_ok = ParsingVector3fAttribute(p_child, "rpy", rpy_value);
        bool xyz_ok = ParsingVector3fAttribute(p_child, "xyz", xyz_value);
        return (rpy_ok && xyz_ok);
    }

    return false;
}

// 更新指定joint的origin
bool UrdfOptApiC::UpdateGivenJointOrigin(TiXmlElement *p_joint,
                                         const boost::array<float, 3> &rpy_value,
                                         const boost::array<float, 3> &xyz_value,
                                         const boost::array<bool, 3> &rpy_upt,
                                         const boost::array<bool, 3> &xyz_upt)
{
    if (nullptr == p_joint)
    {
        return false;
    }

    for (TiXmlElement *p_child = p_joint->FirstChildElement();
         nullptr != p_child; p_child = p_child->NextSiblingElement())
    {
        if ((strcmp("origin", p_child->Value()) != 0))
        {
            continue;
        }

        UpdateVector3fAttribute(p_child, "rpy", rpy_value, rpy_upt);
        UpdateVector3fAttribute(p_child, "xyz", xyz_value, xyz_upt);
        return true;
    }

    return false;
}

// 读取3D矢量属性
bool UrdfOptApiC::ParsingVector3fAttribute(TiXmlElement *p_origin,
                                           const std::string &attribute_name,
                                           boost::array<float, 3> &data_buf)
{
    // 读取原始属性值
    std::string origin_str = p_origin->Attribute(attribute_name.data());

    // 解析属性值
    std::stringstream origin_ss;
    origin_ss << origin_str;
    origin_ss >> data_buf[0] >> data_buf[1] >> data_buf[2];
    return true;
}

// 更新3D矢量属性
void UrdfOptApiC::UpdateVector3fAttribute(TiXmlElement *p_origin,
                                          const std::string &attribute_name,
                                          const boost::array<float, 3> &new_value,
                                          const boost::array<bool, 3> &upt_buf)
{
    // 解析原始值
    boost::array<float, 3> origin_value;
    ParsingVector3fAttribute(p_origin, attribute_name, origin_value);

    // 更新属性值
    std::string new_str;
    for (int idx = 0; idx < 3; ++idx)
    {
        if (upt_buf[idx])
        {
            origin_value[idx] = new_value[idx];
        }
        new_str += std::to_string(origin_value[idx]) + " ";
    }

    p_origin->SetAttribute(attribute_name.data(), new_str.data());
}
