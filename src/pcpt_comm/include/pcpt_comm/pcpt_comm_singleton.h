#ifndef __PCPT_COMM_SINGLETON_H__
#define __PCPT_COMM_SINGLETON_H__

#include "comm/comm_base.h"
#include <mutex>
#include <list>

namespace PCPT
{


/**************************************************************************************
功能描述: 单例管理
修改记录:
**************************************************************************************/
class SINGLETON_MNG_C
{
public:
    using CLR_FUNC_PF = void(*)(void);

    static void RegInstanceClrFunc(CLR_FUNC_PF pfClr);
    static void ClrAllInstances(void);

private:
    static std::mutex             objMutex_;
    static std::list<CLR_FUNC_PF> listClrFunc_;     /* 此处不使用vector，因为其内存扩大后不会释放。 */

    static CLR_FUNC_PF PopLastClrFunc(void);
};


}




#endif
