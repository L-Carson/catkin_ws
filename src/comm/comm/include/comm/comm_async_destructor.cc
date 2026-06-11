#ifndef __COMM_ASYNC_DESTRUCTOR_CC__
#define __COMM_ASYNC_DESTRUCTOR_CC__

/**************************************************************************************
功能描述: 初始化
修改记录:
**************************************************************************************/
template <class T>
INT32 ASYNC_DESTRUCTOR_C<T>::Init(void)
{
    INT32 rslt = CreateTaskThread();
    if (rslt != 0) {
        ST_LOG_ERR("CreateTaskThread() Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 删除对象
修改记录:
**************************************************************************************/
template <class T>
void ASYNC_DESTRUCTOR_C<T>::Del(T *&pobjDel)
{
    if (pobjDel == NULL) {
        ST_LOG_INFO("Not Exist Obj.");
        return;
    }

    PushObjToLst(pobjDel);

    pobjDel = NULL;

    ST_LOG_INFO("Push New Del Obj, Left Num: %u, ", UINT32(lstpobjDel_.size()));
}

/**************************************************************************************
功能描述: 判断是否存在待删除对象
修改记录:
**************************************************************************************/
template <class T>
BOOL  ASYNC_DESTRUCTOR_C<T>::IsExistObj(void)
{
    unique_lock<mutex> objLocker(objMutex_);

    return (lstpobjDel_.size() > 0);
}

/**************************************************************************************
功能描述: 添加待删除对象
修改记录:
**************************************************************************************/
template <class T>
void  ASYNC_DESTRUCTOR_C<T>::PushObjToLst(T *pobjDel)
{
    unique_lock<mutex> objLocker(objMutex_);

    lstpobjDel_.push_back(pobjDel);
}

/**************************************************************************************
功能描述: 获取待删除对象
修改记录:
**************************************************************************************/
template <class T>
T *ASYNC_DESTRUCTOR_C<T>::PopObjFrmLst(void)
{
    unique_lock<mutex> objLocker(objMutex_);

    if (lstpobjDel_.size() <= 0) {
        return NULL;
    }

    T *pobjDel = lstpobjDel_.front();
    lstpobjDel_.pop_front();
    ST_LOG_INFO("Left %u Obj.", UINT32(lstpobjDel_.size()));

    return pobjDel;
}

/**************************************************************************************
功能描述: 单次删除操作
修改记录:
**************************************************************************************/
template <class T>
void  ASYNC_DESTRUCTOR_C<T>::ExecOnce(void)
{
    T *pobjDel = PopObjFrmLst();
    if (pobjDel == NULL) {
        return;
    }

    double startTime = Comm_GetTimeSec();
    ST_LOG_INFO("Del One Obj Start.");
    delete pobjDel;
    ST_LOG_INFO("Del One Obj Succ. Spend Time: %lfs.", Comm_GetTimeSec() - startTime);
}

/**************************************************************************************
功能描述: 线程函数
修改记录:
**************************************************************************************/
template <class T>
void  ASYNC_DESTRUCTOR_C<T>::ExecTask(void)
{
    ST_LOG_INFO("Begin.");

    while (isThreadTaskWork_ || IsExistObj()) {
        ExecOnce();

        usleep(int(0.005 * 1000 * 1000));
    }

    ST_LOG_INFO("End.");
}

/**************************************************************************************
功能描述: 创建任务线程
修改记录:
**************************************************************************************/
template <class T>
INT32 ASYNC_DESTRUCTOR_C<T>::CreateTaskThread(void)
{
    isThreadTaskWork_ = true;

    pobjThread_ = new thread(&ASYNC_DESTRUCTOR_C::ExecTask, this);
    if (pobjThread_ == NULL) {
        ST_LOG_ERR("New Task Thread Fail.");
        return -1;
    }

    ST_LOG_INFO("Succ.");
    return 0;
}

/**************************************************************************************
功能描述: 删除任务线程
修改记录:
**************************************************************************************/
template <class T>
void  ASYNC_DESTRUCTOR_C<T>::DeleteTaskThread(void)
{
    ST_LOG_INFO("Begin.");

    isThreadTaskWork_ = false;

    if (pobjThread_ != NULL) {
        pobjThread_->join();

        delete pobjThread_;
        pobjThread_ = NULL;
    }

    ST_LOG_INFO("Succ.");
}

#endif

