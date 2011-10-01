#ifndef TIMEMGR_H
#define TIMEMGR_H

#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "shared/iSingleton.h"
#include "shared/typedef.h"
#include "shared/time/cTime.h"

#include <list>

namespace jRylServer {
namespace common {
namespace shared {
namespace time {

class TimeMgr : public Singleton<TimeMgr> {
public:
    MAKE_SINGLETON(TimeMgr);
    void Stop();
    bool Start();

    Time* CreateTime(const char* name, uint32 seconds, Time::TimeCallBack callBack);
    Time* CreateTime(const char* name, uint32 seconds);
    Time* CreateTime(uint32 seconds);
    Time* CreateTime(uint32 seconds, Time::TimeCallBack callBack);
    void StopTime(const char* name);
    void StopTime(Time* time);

    ~TimeMgr();
private:
    TimeMgr();
    void AsynAsioCallBack(const boost::system::error_code& error, Time* time);

    boost::mutex m_timesMutex;
    std::list<Time*> m_times;
    boost::asio::io_service m_service;
    boost::asio::io_service::work* m_serviceWork;
    boost::thread m_serviceThread;
};

} //namespace time
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif