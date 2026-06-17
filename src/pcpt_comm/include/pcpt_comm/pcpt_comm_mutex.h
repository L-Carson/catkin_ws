#pragma once
#include <mutex>

class PCPT_MUTEX_C
{
public:
    PCPT_MUTEX_C(){}
    ~PCPT_MUTEX_C(){}

    void pcpt_mutex_main_lock() {
        mutex_main.lock();
    }
    
    void pcpt_mutex_main_unlock() {
        mutex_main.unlock();
    }

private:
    std::mutex mutex_main;
};