

#include "pcpt_comm/pcpt_comm_singleton.h"


namespace PCPT
{

std::mutex                              SINGLETON_MNG_C::objMutex_;
std::list<SINGLETON_MNG_C::CLR_FUNC_PF> SINGLETON_MNG_C::listClrFunc_;

void SINGLETON_MNG_C::RegInstanceClrFunc(CLR_FUNC_PF pfClr)
{
    R_ASSERT(pfClr != nullptr, NOTHING);

    std::unique_lock<std::mutex> objLocker(objMutex_);

    listClrFunc_.push_back(pfClr);
    ST_LOG_INFO("Add New Clr Func: %p, Total Num Is %u.", pfClr, UINT32(listClrFunc_.size()));
}

void SINGLETON_MNG_C::ClrAllInstances(void)
{
    ST_LOG_INFO("Begin...");

    while (true) {
        CLR_FUNC_PF pfClr = PopLastClrFunc();
        if (pfClr == nullptr) {
            break;
        }

        pfClr();
        ST_LOG_INFO("Clr %p Succ.", pfClr);
    }

    ST_LOG_INFO("End.");
}

SINGLETON_MNG_C::CLR_FUNC_PF SINGLETON_MNG_C::PopLastClrFunc(void)
{
    std::unique_lock<std::mutex> objLocker(objMutex_);

    if (listClrFunc_.empty()) {
        return nullptr;
    }

    CLR_FUNC_PF pfLastClr = listClrFunc_.back();
    listClrFunc_.pop_back();

    ST_LOG_INFO("Pop Clr Func: %p, Left Num Is %u.", pfLastClr, UINT32(listClrFunc_.size()));
    return pfLastClr;
}




}

