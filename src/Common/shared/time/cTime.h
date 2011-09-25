#ifndef TIME_H
#define TIME_H

#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace jRylServer {
namespace common {
namespace shared {
namespace time {

class Time {
public:

    enum TimeError {
        TE_Success,
        TE_Canceled,
        TE_Error
    };

    typedef boost::function<void(Time::TimeError, const char*) > TimeCallBack;

    Time(boost::asio::io_service& service);
    void SetName(const char* name);
    const char* GetName();
    void SetCallBack(TimeCallBack callBack);
    void ExecuteCallBack(TimeError error, const char* errMsg);
    boost::asio::deadline_timer* GetTimeObj();
private:
    char m_name[30];
    TimeCallBack m_timeCallBack;
    boost::asio::deadline_timer m_timeObject;
};

} //namespace time
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif