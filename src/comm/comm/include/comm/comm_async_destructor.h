#ifndef __COMM_ASYNC_DESTRUCTOR_H__
#define __COMM_ASYNC_DESTRUCTOR_H__
#include "comm/comm_data_type.h"
#include "comm/comm_dbg_log.h"

#include <list>
#include <thread>
#include <mutex>
#include <atomic>


using namespace std;


/**************************************************************************************
功能描述: 异步删除耗时阻塞对象
修改记录:
**************************************************************************************/
template <class T>
class ASYNC_DESTRUCTOR_C
{
public:
    ASYNC_DESTRUCTOR_C(void) : pobjThread_(NULL), isThreadTaskWork_(true)
    {
    }

    ~ASYNC_DESTRUCTOR_C(void)
    {
        DeleteTaskThread();
    }

    INT32 Init(void);
    void  Del(T *&pobjDel);

private:
    thread      *pobjThread_;
    mutex        objMutex_;
    atomic_bool  isThreadTaskWork_;                                     /* 多线程变量 *//* 是否线程任务是否工作 */
    list<T *>    lstpobjDel_;

    BOOL  IsExistObj(void);
    void  PushObjToLst(T *pobjDel);
    T    *PopObjFrmLst(void);

    void  ExecOnce(void);
    void  ExecTask(void);

    INT32 CreateTaskThread(void);
    void  DeleteTaskThread(void);
};

#include "comm_async_destructor.cc"

#endif

