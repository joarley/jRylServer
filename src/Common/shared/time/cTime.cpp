#include "cTime.h"

#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace jRylServer {
namespace common {
namespace shared {
namespace time {


Time::Time(boost::asio::io_service& service): m_timeObject(service, boost::posix_time::seconds(0)) {
    *m_name = 0;
    m_timeCallBack = NULL;
}

void Time::SetName(const char* name) {
    if(name != NULL) {
        strcpy(m_name, name);
    }
}

const char* Time::GetName() {
    return m_name;
}

void Time::SetCallBack(TimeCallBack callBack) {
    m_timeCallBack = callBack;
}

void Time::ExecuteCallBack(TimeError error, const char* errMsg) {
    if(m_timeCallBack != NULL) {
        m_timeCallBack(error, errMsg);
    }
}

boost::asio::deadline_timer* Time::GetTimeObj() {
    return &m_timeObject;
}


} //namespace time
} //namespace shared
} //namespace common
} //namespace jRylServer