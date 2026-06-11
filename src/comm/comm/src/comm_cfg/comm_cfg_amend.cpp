#include "comm/comm_cfg/comm_cfg_amend.h"

namespace COMM
{

namespace CFG
{

/**************************************************************************************
功能描述: Element to String
修改记录:
**************************************************************************************/
const char* CFG_AMEND_C::Element2Str(const std::initializer_list<const char*> element) const
{
    std::stringstream ss_data;
    for (auto iter = element.begin(); iter != element.end();) {
        ss_data << *iter;
        if (++iter != element.end()) ss_data << "-";
    }

    static string strEle;
    strEle = ss_data.str();
    return std::move(strEle.c_str());
}

/**************************************************************************************
功能描述: 获取节点
修改记录: 当前节点下，有多个同名节点时，可指定获取第几个，默认读取第一个
**************************************************************************************/
TiXmlElement* CFG_AMEND_C::GetElement(TiXmlElement& xmlElem, const char* element, const int index) const
{
    return GetElement(xmlElem, {element}, index);
}

TiXmlElement* CFG_AMEND_C::GetElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const int index) const
{
    if (index < 0) {
        ST_LOG_ERR("Please set the correct index, The current index is:%d", index);
        return nullptr;
    }
    if (!element.size()) return &xmlElem;

    auto iter = element.begin();
    TiXmlElement* pElem     = &xmlElem;
    TiXmlElement* pElemNext = xmlElem.FirstChildElement(*iter);

    std::stringstream ss_data;

    int indexT = index;
    do {
        ss_data << *iter;
        if (!pElemNext) return nullptr;
        auto iterNext = next(iter, 1);
        if (iterNext == element.end()) {
            while (--indexT >= 0) {
                pElemNext = pElemNext->NextSiblingElement();
            }
            return pElemNext;
        } else {
            iter = iterNext++;
            pElem = pElemNext;
            pElemNext = pElem->FirstChildElement(*iter);
        }
        ss_data << "-";
    } while (1);

    return nullptr;
}

/**************************************************************************************
功能描述: 获取当前节点下同名节点的数量
修改记录:
**************************************************************************************/
int CFG_AMEND_C::SiblingElementCount(TiXmlElement& xmlElem, const char* element) const
{
    return SiblingElementCount(xmlElem, {element});
}

int CFG_AMEND_C::SiblingElementCount(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const
{
    TiXmlElement* pElem = GetElement(xmlElem, element);
    int count = 0;
    do {
        if (!pElem) return count;
        pElem = pElem->NextSiblingElement();
        count++;
    } while(1);

    return 0;
}

/**************************************************************************************
功能描述: 节点是否存在
修改记录:
**************************************************************************************/
bool CFG_AMEND_C::IsElementExist(TiXmlElement& xmlElem, const char* element) const
{
    return IsElementExist(xmlElem, {element});
}

bool CFG_AMEND_C::IsElementExist(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const
{
    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) return false;
    return true;
}

/**************************************************************************************
功能描述: 添加节点
修改记录:
**************************************************************************************/
TiXmlElement* CFG_AMEND_C::AddElement(TiXmlElement& xmlElem, const char* element, const bool isSibling) const
{
    return AddElement(xmlElem, {element}, isSibling);
}

TiXmlElement* CFG_AMEND_C::AddElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const bool isSibling) const
{
    //if (IsElementExist(xmlElem, element)) return GetElement(xmlElem, element);
    if (!element.size()) return &xmlElem;
    auto iter = element.begin();
    TiXmlElement* pElem     = &xmlElem;
    TiXmlElement* pElemNext{nullptr};

    std::stringstream ss_data;

    do {
        ss_data << *iter;

        if (IsElementExist(*pElem, *iter)) {
            pElemNext = GetElement(*pElem, *iter);
            if (++iter == element.end()) {
                if (!isSibling) return pElemNext;
                pElemNext = new TiXmlElement(*(--iter));
                if (!pElemNext) {
                    ST_LOG_ERR("Failed to Add Sibling Element <%s>.", ss_data.str().c_str());
                    return nullptr;
                }
                pElem->LinkEndChild(pElemNext);
                return pElemNext;
            }
            pElem = pElemNext;
            ss_data << "-";
            continue;
        }

        pElemNext = new TiXmlElement(*iter);
        if (!pElemNext) {
            ST_LOG_ERR("Failed to Add Element <%s>.", ss_data.str().c_str());
            return nullptr;
        }
        pElem->LinkEndChild(pElemNext);

        auto iterNext = next(iter, 1);
        if (iterNext == element.end()) {
            ST_LOG_INFO("Succeed to Add Element <%s>.", ss_data.str().c_str());
            return pElemNext;
        } else {
            iter = iterNext++;
            pElem = pElemNext;
        }

        ss_data << "-";
    } while (1);

    return nullptr;
}

/**************************************************************************************
功能描述: 删除节点
修改记录:
**************************************************************************************/
int CFG_AMEND_C::DeleteElement(TiXmlElement& xmlElem, const char* element) const
{
    return DeleteElement(xmlElem, {element});
}

int CFG_AMEND_C::DeleteElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element) const
{
    if (!IsElementExist(xmlElem, element)) {
        ST_LOG_WARN("The Element <%s> is not Exist.", Element2Str(element));
        return 0;
    }

    auto iter = element.begin();
    TiXmlElement* pElem     = &xmlElem;
    TiXmlElement* pElemNext = xmlElem.FirstChildElement(*iter);

    std::stringstream ss_data;

    do {
        ss_data << *iter;

        if (!pElemNext) {
            ST_LOG_ERR("Failed to Find Element <%s>.", ss_data.str().c_str());
            return -1;
        }

        auto iterNext = next(iter, 1);
        if (iterNext == element.end()) {
            if (pElem->RemoveChild(pElemNext)) {
                ST_LOG_INFO("Succeed to Delete Element <%s>.", ss_data.str().c_str());
                return 0;
            } else {
                ST_LOG_ERR("Failed to Delete Element <%s>.", ss_data.str().c_str());
                return -1;
            }
        } else {
            iter = iterNext++;
            pElem = pElemNext;
            pElemNext = pElem->FirstChildElement(*iter);
        }

        ss_data << "-";
    } while (1);

    return -1;
}

/**************************************************************************************
功能描述: 节点重命名
修改记录:
**************************************************************************************/
int CFG_AMEND_C::RenameElement(TiXmlElement& xmlElem, const char* element, const char *name) const
{
    return RenameElement(xmlElem, {element}, name);
}

int CFG_AMEND_C::RenameElement(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *name) const
{
    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) {
        ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
        return -1;
    }
    pElem->SetValue(name);
    auto iter = element.end();
    ST_LOG_INFO("Succeed to Amend the Element Name form <%s> to <%s>.", *(--iter), name);

    return 0;
}

/**************************************************************************************
功能描述: 节点属性是否存在
修改记录:
**************************************************************************************/
bool CFG_AMEND_C::IsAttributeExist(TiXmlElement& xmlElem, const char* element, const char *attr) const
{
    return IsAttributeExist(xmlElem, {element}, attr);
}

bool CFG_AMEND_C::IsAttributeExist(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const
{
    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) return false;
    if (!pElem->Attribute(attr)) return false;
    return true;
}

/**************************************************************************************
功能描述: 删除节点
修改记录:
**************************************************************************************/
int CFG_AMEND_C::DeleteAttribute(TiXmlElement& xmlElem, const char* element, const char *attr) const
{
    return DeleteAttribute(xmlElem, {element}, attr);
}

int CFG_AMEND_C::DeleteAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const
{
    if (!IsAttributeExist(xmlElem, element, attr)) {
        ST_LOG_WARN("Attr <%s> of the Element <%s> is not Exist.", attr, Element2Str(element));
        return 0;
    }

    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) {
        ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
        return -1;
    }

    pElem->RemoveAttribute(attr);
    auto iter = element.end();
    ST_LOG_INFO("Succeed to Delete the Attribute <%s>-<%s>.", *(--iter), attr);

    return 0;
}

/**************************************************************************************
功能描述: 节点属性重命名
修改记录:
**************************************************************************************/
int CFG_AMEND_C::RenameAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const char *name) const
{
    return RenameAttribute(xmlElem, {element}, attr, name);
}

int CFG_AMEND_C::RenameAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const char *name) const
{
    if (!IsAttributeExist(xmlElem, element, attr)) {
        ST_LOG_ERR("Attr <%s> of the Element <%s> is not Exist.", attr, Element2Str(element));
        return -1;
    }

    if (IsAttributeExist(xmlElem, element, name)) {
        ST_LOG_WARN("Attr <%s> of the Element <%s> is has Exist.", attr, Element2Str(element));
        return 0;
    }

    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) {
        ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
        return -1;
    }

    TiXmlAttribute *attrib = pElem->FirstAttribute();

    do {
        const char* attrName = attrib->Name();
        if (!attrName) {
            ST_LOG_ERR("Attr <%s> of the Element <%s> is not Exist.", attr, Element2Str(element));
            return -1;
        }

        if (strcmp(attr, attrName)) {
            attrib = attrib->Next();
            continue;
        } else {
            attrib->SetName(name);
            ST_LOG_INFO("Succeed to Amend <%s> Attr Name form <%s> to <%s>.", Element2Str(element), attr, name);
            return 0;
        }
    } while(1);

    return -1;
}

/**************************************************************************************
功能描述: 节点属性值是否为true
修改记录:
**************************************************************************************/
bool CFG_AMEND_C::IsAttributeTrue(TiXmlElement& xmlElem, const char* element, const char *attr) const
{
    return IsAttributeTrue(xmlElem, {element}, attr);
}

bool CFG_AMEND_C::IsAttributeTrue(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr) const
{
    bool isSupport = false;
    if (GetAttribute(xmlElem, element, attr, isSupport)) return false;
    return isSupport;
}

/**************************************************************************************
功能描述: 获取节点属性值
修改记录:
**************************************************************************************/
int CFG_AMEND_C::GetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, bool &value) const
{
    return GetAttribute(xmlElem, {element}, attr, value);
}

int CFG_AMEND_C::GetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, bool &value) const
{
    TiXmlElement* pElem = GetElement(xmlElem, element);
    if (!pElem) {
        ST_LOG_ERR("Failed to Get Element <%s>.", Element2Str(element));
        return -1;
    }
    if(TIXML_SUCCESS != pElem->QueryBoolAttribute(attr, &value)) {
        ST_LOG_ERR("Failed to Get <%s> Attr <%s>.", Element2Str(element), attr);
        return -1;
    }
    return 0;
}

/**************************************************************************************
功能描述: 添加节点属性值
修改记录:
**************************************************************************************/
int CFG_AMEND_C::AddAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const float value) const
{
    return AddAttribute(xmlElem, {element}, attr, value);
}

int CFG_AMEND_C::AddAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const double value) const
{
    return AddAttribute(xmlElem, {element}, attr, value);
}

int CFG_AMEND_C::AddAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const float value) const
{
    return AddAttribute(xmlElem, element, attr, double(value));
}

int CFG_AMEND_C::AddAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const double value) const
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
    pElem->SetDoubleAttribute(attr, value);

    std::stringstream ss_data;
    ss_data << value;
    ST_LOG_INFO("Succeed to Add <%s> Attr <%s> Value:%s.", Element2Str(element), attr, ss_data.str().c_str());
    return 0;
}

/**************************************************************************************
功能描述: 修改节点属性值
修改记录:
**************************************************************************************/
int CFG_AMEND_C::SetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const float value) const
{
    return SetAttribute(xmlElem, {element}, attr, value);
}

int CFG_AMEND_C::SetAttribute(TiXmlElement& xmlElem, const char* element, const char *attr, const double value) const
{
    return SetAttribute(xmlElem, {element}, attr, value);
}

int CFG_AMEND_C::SetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const float value) const
{
    return SetAttribute(xmlElem, element, attr, double(value));
}

int CFG_AMEND_C::SetAttribute(TiXmlElement& xmlElem, const std::initializer_list<const char*> element, const char *attr, const double value) const
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
    pElem->SetDoubleAttribute(attr, value);

    std::stringstream ss_data;
    ss_data << value;
    ST_LOG_INFO("Succeed to Set <%s> Attr <%s> Value:%s.", Element2Str(element), attr, ss_data.str().c_str());
    return 0;
}


}

}

