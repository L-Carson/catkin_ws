#ifndef __COMM_CFG_PARAM_READ_BASE_H__
#define __COMM_CFG_PARAM_READ_BASE_H__

#include "comm_cfg_param_define.h"

#include "comm/comm_base.h"
#include "comm/loc_xml/tinyxml.h"

#include <math.h>
#include <string>
#include <vector>
#include <list>
#include <map>

/**************************************************************************************
功能描述: 读取指定属性
修改记录:
**************************************************************************************/
#define READ_ATTR(pobjXml_, pfGetAttr_, cfgName_, attrName_, attrVal_)                  \
    if (TIXML_SUCCESS != pobjXml_->pfGetAttr_(attrName_, &attrVal_)) {                  \
        ST_LOG_ERR("Get <" cfgName_ "> Cfg Attr <" attrName_ "> Fail.");                \
        return -1;                                                                      \
    }

#define READ_ATTR_UINT32(pobjXml_, cfgName_, attrName_, attrVal_)   READ_ATTR(pobjXml_, QueryUnsignedAttribute, cfgName_, attrName_, attrVal_)
#define READ_ATTR_FLOAT( pobjXml_, cfgName_, attrName_, attrVal_)   READ_ATTR(pobjXml_, QueryFloatAttribute,    cfgName_, attrName_, attrVal_)
#define READ_ATTR_BOOL(  pobjXml_, cfgName_, attrName_, attrVal_)   READ_ATTR(pobjXml_, QueryBoolAttribute,     cfgName_, attrName_, attrVal_)
#define READ_ATTR_STRING(pobjXml_, cfgName_, attrName_, attrVal_)   READ_ATTR(pobjXml_, QueryStringAttribute,   cfgName_, attrName_, attrVal_)


/**************************************************************************************
功能描述: 读取指定配置
修改记录:
**************************************************************************************/
#define READ_CFG(pfRead_, pobjXml_, cfgVal_)                    \
    if (0 != pfRead_(pobjXml_, cfgVal_)) {                      \
        ST_LOG_ERR(#pfRead_"() Fail.");                         \
        return -1;                                              \
    }


namespace COMM
{

namespace CFG
{


/**************************************************************************************
功能描述: 配置参数读取
修改记录:
**************************************************************************************/
class CFG_PARAM_READ_BASE_C
{
public:
protected:
    template<typename T>
    INT32 GetAttribute(TiXmlElement *pobjCfgParam, const char *attribute, T &value)
    {
        if (!pobjCfgParam) {
            ST_LOG_ERR("The element to which <%s> belongs dose not exist.", attribute);
            return -1;
        }

        const char* element = pobjCfgParam->Value();

        if (TIXML_SUCCESS != pobjCfgParam->QueryValueAttribute(attribute, &value)) {
            ST_LOG_ERR("Get <%s> Attribute <%s> Fail.", element, attribute);
            return -1;
        }
        return 0;
    }

    INT32 Char2Num(char c);
    INT32 StringHex2Num(string str);

    INT32 GetAttribute(TiXmlElement *pobjCfgParam, const char *attribute, bool &value);

    TiXmlElement *GetFirstChildElement(TiXmlElement *pobjCfgParam, const char *element);
    TiXmlElement *GetNextSiblingElement(TiXmlElement *pobjCfgParam, const char *element);

    BOOL  IsValidSensor(const vector<string>& vstr, const string str) const;

    INT32 ReadRectangleCfg(TiXmlElement *pobjRectangleCfg, const char *element, RECTANGLE_STRU &stRect);
    INT32 ReadCuboidCfg(TiXmlElement *pobjCuboidCfg, const char *element, CUBOID_STRU &stCuboid);


private:
};

}

}

#endif

