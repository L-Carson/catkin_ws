
#include "comm/comm_cfg/comm_cfg_param_read_base.h"

namespace COMM
{

namespace CFG
{


/**************************************************************************************
功能描述: ASCII转整型
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_BASE_C::Char2Num(char c)
{
    if (c >= '0' && c <= '9') {
        return  c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        return -1;
    }
}

/**************************************************************************************
功能描述: string hex to num
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_BASE_C::StringHex2Num(string str)
{
    if (!str.size()) return -1;
    if (str.size() > 2 && (str[1] == 'x' || str[1] == 'X'))
        str = str.substr(2, str.size()-2);

    int rst = 0;
    for (char c : str) {
        int num = Char2Num(c);
        if (num < 0) return -1;
        rst = rst * 16 + Char2Num(c);
    }
    return rst;
}

INT32 CFG_PARAM_READ_BASE_C::GetAttribute(TiXmlElement *pobjCfgParam, const char *attribute, bool &value)
{
    if (!pobjCfgParam) {
        ST_LOG_ERR("The element to which <%s> belongs dose not exist.", attribute);
        return -1;
    }

    const char *element = pobjCfgParam->Value();

    if (TIXML_SUCCESS != pobjCfgParam->QueryBoolAttribute(attribute, &value)) {
        ST_LOG_ERR("Get <%s> Attribute <%s> Fail.", element, attribute);
        return -1;
    }
    return 0;
}

TiXmlElement *CFG_PARAM_READ_BASE_C::GetFirstChildElement(TiXmlElement *pobjCfgParam, const char *element)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("pobjCfgParam==NULL.");
        return NULL;
    }

    TiXmlElement *pTiXmlElement = pobjCfgParam->FirstChildElement(element);

    if(pTiXmlElement == NULL) {
        ST_LOG_ERR("Get <%s> Element <%s> Fail.", pobjCfgParam->Value(), element);
        return NULL;
    }

    return pTiXmlElement;
}

TiXmlElement *CFG_PARAM_READ_BASE_C::GetNextSiblingElement(TiXmlElement *pobjCfgParam, const char *element)
{
    if (pobjCfgParam == NULL) {
        ST_LOG_ERR("pobjCfgParam==NULL.");
        return NULL;
    }

    TiXmlElement *pTiXmlElement = pobjCfgParam->NextSiblingElement(element);

    if(pTiXmlElement == NULL) {
        ST_LOG_ERR("Get <%s> Element <%s> Fail.", pobjCfgParam->Value(), element);
        return NULL;
    }

    return pTiXmlElement;
}

/**************************************************************************************
功能描述: 是否为有效的传感器
修改记录:
**************************************************************************************/
BOOL CFG_PARAM_READ_BASE_C::IsValidSensor(const vector<string>& vstr, const string str) const
{
    return HasElement(vstr, str);
}

/**************************************************************************************
功能描述: 读取矩形框参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_BASE_C::ReadRectangleCfg(TiXmlElement *pobjCfgParam, const char *element, RECTANGLE_STRU &stRect)
{
    TiXmlElement *pobjRegion = GetFirstChildElement(pobjCfgParam, element);
    if (pobjRegion == NULL) return -1;

    int rslt = 0;
    rslt += GetAttribute(pobjRegion, "MinX", stRect.minX);
    rslt += GetAttribute(pobjRegion, "MaxX", stRect.maxX);
    rslt += GetAttribute(pobjRegion, "MinY", stRect.minY);
    rslt += GetAttribute(pobjRegion, "MaxY", stRect.maxY);

    if (rslt) return -1;
    //ST_LOG_INFO("MinX:%f, MaxX:%f, MinY:%f, MaxY:%f", stRect.minX, stRect.maxX, stRect.minY, stRect.maxY);
    return 0;
}

/**************************************************************************************
功能描述: 读取长方体参数
修改记录:
**************************************************************************************/
INT32 CFG_PARAM_READ_BASE_C::ReadCuboidCfg(TiXmlElement *pobjCfgParam, const char *element, CUBOID_STRU &stCuboid)
{
    TiXmlElement *pobjCuboid = GetFirstChildElement(pobjCfgParam, element);
    if (pobjCuboid == NULL) return -1;

    int rslt = 0;
    rslt += GetAttribute(pobjCuboid, "MinX", stCuboid.minX);
    rslt += GetAttribute(pobjCuboid, "MaxX", stCuboid.maxX);
    rslt += GetAttribute(pobjCuboid, "MinY", stCuboid.minY);
    rslt += GetAttribute(pobjCuboid, "MaxY", stCuboid.maxY);
    rslt += GetAttribute(pobjCuboid, "MinZ", stCuboid.minZ);
    rslt += GetAttribute(pobjCuboid, "MaxZ", stCuboid.maxZ);

    if (rslt) return -1;
    //ST_LOG_INFO("MinX:%f, MaxX:%f, MinY:%f, MaxY:%f, MinZ:%f, MaxZ:%f", stCuboid.minX, stCuboid.maxX, stCuboid.minY, stCuboid.maxY, stCuboid.minZ, stCuboid.maxZ);
    return 0;
}



}
}

