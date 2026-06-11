#ifndef __COMM_CFG_AMEND_H__
#define __COMM_CFG_AMEND_H__

#include "comm/comm_base.h"
#include "comm/comm_file.h"
#include "comm/loc_xml/tinyxml.h"

#include <string>
#include <sstream>
#include <initializer_list>

using namespace std;

namespace COMM
{

namespace CFG
{

/**************************************************************************************
功能描述: 配置文件修订
修改记录:
**************************************************************************************/
class CFG_AMEND_C
{
public:
    CFG_AMEND_C() {}
    ~CFG_AMEND_C() {}

    /**
     * Element
     */
    const char* Element2Str(const std::initializer_list<const char*> element) const;

    TiXmlElement* GetElement(TiXmlElement& xmlElem, const char* element, const int index = 0) const;
    TiXmlElement* GetElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const int index = 0) const;

    int SiblingElementCount(TiXmlElement& xmlElem, const char* element) const;
    int SiblingElementCount(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const;

    bool IsElementExist(TiXmlElement& xmlElem, const char* element) const;
    bool IsElementExist(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const;

    TiXmlElement* AddElement(TiXmlElement& xmlElem, const char* element, const bool isSibling = false) const;
    TiXmlElement* AddElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const bool isSibling = false) const;

    int DeleteElement(TiXmlElement& xmlElem, const char* element) const;
    int DeleteElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const;

    int RenameElement(TiXmlElement& xmlElem, const char* element, const char *name) const;
    int RenameElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *name) const;

    /**
     * Attribute
     */
    bool IsAttributeExist(TiXmlElement& xmlElem, const char* element, const char *attr) const;
    bool IsAttributeExist(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const;

    int  DeleteAttribute(TiXmlElement& xmlElem, const char* element, const char *attr) const;
    int  DeleteAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const;

    int  RenameAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const char *name) const;
    int  RenameAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const char *name) const;

    bool IsAttributeTrue(TiXmlElement& xmlElem, const char* element, const char *attr) const;
    bool IsAttributeTrue(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const;

    /* 获取节点属性值 */
    int GetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, bool &value) const;
    int GetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, bool &value) const;

    template<typename T>
    int GetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, T &value) const
    {
        return GetAttribute(xmlElem, {element}, attr, value);
    }

    template<typename T>
    int GetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, T &value) const
    {
        TiXmlElement* pElem = GetElement(xmlElem, element);
        if (!pElem) {
            ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
            return -1;
        }
        if(TIXML_SUCCESS != pElem->QueryValueAttribute(attr, &value)) {
            ST_LOG_ERR("Failed to Get <%s> Attr <%s>.", Element2Str(element), attr);
            return -1;
        }
        return 0;
    }

    /* 添加节点属性 */
    int AddAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const float value) const;
    int AddAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const double value) const;
    int AddAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const float value) const;
    int AddAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const double value) const;

    template<typename T>
    int AddAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const T value) const
    {
        return AddAttribute(xmlElem, {element}, attr, value);
    }

    template<typename T>
    int AddAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const T value) const
    {
        if (IsAttributeExist(xmlElem, element, attr)) {
            std::stringstream ss_data;
            ss_data << value;
            ST_LOG_WARN("Attr <%s> of the Element <%s> has Already Existed, and Value is:%s.", attr, Element2Str(element), ss_data.str().c_str());
            return 0;
        }

        TiXmlElement* pElem = AddElement(xmlElem, element);
        if (!pElem) {
            ST_LOG_ERR("Failed to Add Element <%s>.", Element2Str(element));
            return -1;
        }
        pElem->SetAttribute(attr, value);

        std::stringstream ss_data;
        ss_data << value;
        ST_LOG_INFO("Succeed to Add <%s> Attr <%s> Value:%s.", Element2Str(element), attr, ss_data.str().c_str());
        return 0;
    }

    /* 修改节点属性值，如果不存在则创建新的属性 */
    int SetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const float value) const;
    int SetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const double value) const;
    int SetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const float value) const;
    int SetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const double value) const;

    template<typename T>
    int SetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const T value) const
    {
        return SetAttribute(xmlElem, {element}, attr, value);
    }

    template<typename T>
    int SetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const T value) const
    {
        if (!IsAttributeExist(xmlElem, element, attr)) {
            ST_LOG_WARN("Attr <%s> of the Element <%s> is not Exist, and a New One Will be Created.", attr, Element2Str(element));
            if (AddAttribute(xmlElem, element, attr, value)) return -1;
            return 0;
        }

        TiXmlElement* pElem = GetElement(xmlElem, element);
        if (!pElem) {
            ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
            return -1;
        }
        pElem->SetAttribute(attr, value);

        std::stringstream ss_data;
        ss_data << value;
        ST_LOG_INFO("Succeed to Set <%s> Attr <%s> Value:%s.", Element2Str(element), attr, ss_data.str().c_str());
        return 0;
    }
};

}

}

#endif
