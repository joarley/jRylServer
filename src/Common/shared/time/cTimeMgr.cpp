#include "cTimeMgr.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "shared/time/cTime.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

#include <list>
#include <cstring>
#include <vector>

namespace jRylServer {
namespace common {
namespace shared {
namespace time {

TimeMgr::TimeMgr() {
}

TimeMgr::~TimeMgr() {
}

bool TimeMgr::Start() {
    m_serviceWork = new boost::asio::io_service::work(m_service);
    m_serviceThread = boost::thread(boost::bind(&boost::asio::io_service::run, &m_service));
    return true;
}

void TimeMgr::Stop() {
    std::vector<Time*> times;

    BOOST_FOREACH(Time* t, m_times) {
        times.push_back(t);
    }

    for (int i = 0; i < (int) times.size(); i++) {
        boost::system::error_code ec;
        times[i]->GetTimeObj()->cancel(ec);
    }

    delete m_serviceWork;
    m_service.stop();
    m_serviceThread.join();
}

Time* TimeMgr::CreateTime(const char* name, uint32 seconds, Time::TimeCallBack callBack) {
    Time* time = new Time(m_service);
    time->SetName(name);
    time->SetCallBack(callBack);
    boost::asio::deadline_timer* tObj = time->GetTimeObj();
    boost::system::error_code ec;
    tObj->expires_at(tObj->expires_at() + boost::posix_time::seconds(seconds), ec);
    if (ec) {
        Logger::GetInstance().ShowError("CreateTime failed -> %s", ec.message().c_str());
        return NULL;
    }
    tObj->async_wait(boost::bind(&TimeMgr::AsynAsioCallBack, this, boost::asio::placeholders::error, time));
    m_times.push_back(time);
    return time;
}

Time* TimeMgr::CreateTime(const char* name, uint32 seconds) {
    Time* time = new Time(m_service);
    time->SetName(name);
    time->SetCallBack(NULL);
    boost::asio::deadline_timer* tObj = time->GetTimeObj();
    boost::system::error_code ec;
    tObj->expires_at(tObj->expires_at() + boost::posix_time::seconds(seconds), ec);
    if (ec) {
        Logger::GetInstance().ShowError("CreateTime failed -> %s", ec.message().c_str());
        return NULL;
    }
    tObj->async_wait(boost::bind(&TimeMgr::AsynAsioCallBack, this, boost::asio::placeholders::error, time));
    m_times.push_back(time);
    return time;
}

Time* TimeMgr::CreateTime(uint32 seconds) {
    Time* time = new Time(m_service);
    time->SetName(NULL);
    time->SetCallBack(NULL);
    boost::asio::deadline_timer* tObj = time->GetTimeObj();
    boost::system::error_code ec;
    tObj->expires_at(tObj->expires_at() + boost::posix_time::seconds(seconds), ec);
    if (ec) {
        Logger::GetInstance().ShowError("CreateTime failed -> %s", ec.message().c_str());
        return NULL;
    }
    tObj->async_wait(boost::bind(&TimeMgr::AsynAsioCallBack, this, boost::asio::placeholders::error, time));
    m_times.push_back(time);
    return time;
}

Time* TimeMgr::CreateTime(uint32 seconds, Time::TimeCallBack callBack) {
    Time* time = new Time(m_service);
    time->SetName(NULL);
    time->SetCallBack(callBack);
    boost::asio::deadline_timer* tObj = time->GetTimeObj();
    boost::system::error_code ec;
    tObj->expires_at(tObj->expires_at() + boost::posix_time::seconds(seconds), ec);
    if (ec) {
        Logger::GetInstance().ShowError("CreateTime failed -> %s", ec.message().c_str());
        return NULL;
    }
    tObj->async_wait(boost::bind(&TimeMgr::AsynAsioCallBack, this, boost::asio::placeholders::error, time));
    m_times.push_back(time);
    return time;
}

void TimeMgr::StopTime(const char* name) {
    Time* time = NULL;

    BOOST_FOREACH(Time* t, m_times) {
        if (strcmp(name, t->GetName()) == 0) {
            time = t;
            break;
        }
    }
    if (time != NULL) {
        boost::system::error_code ec;
        time->GetTimeObj()->cancel(ec);
        m_times.remove(time);
    }
}

void TimeMgr::StopTime(Time* time) {
    if (time != NULL) {
        boost::system::error_code ec;
        time->GetTimeObj()->cancel(ec);
        m_times.remove(time);
    }
}

void TimeMgr::AsynAsioCallBack(const boost::system::error_code& error, Time* time) {
    if (error == boost::asio::error::operation_aborted) {
        time->ExecuteCallBack(Time::TE_Canceled, NULL);
    } else {
        time->ExecuteCallBack(Time::TE_Success, NULL);
    }
    m_times.remove(time);
    delete time;
}

} //namespace time
} //namespace shared
} //namespace common
} //namespace jRylServer