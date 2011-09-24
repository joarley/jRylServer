#include "utils.h"

#include <boost/thread/mutex.hpp>
#include "shared/typedef.h"
#include <ctime>

namespace jRylServer {
namespace common {
namespace shared {
void j_GetLocalTime(tm* tmResult) {
    static boost::mutex getLocalTimeMutex;
    getLocalTimeMutex.lock();
    time_t aux1;
    time(&aux1);
    tm* aux2 = localtime(&aux1);
    memcpy(tmResult, aux2, sizeof(tm));
    getLocalTimeMutex.unlock();
}
} //namespace shared
} //namespace common
} //namespace jRylServer