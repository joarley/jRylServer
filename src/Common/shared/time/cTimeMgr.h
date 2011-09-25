#ifndef TIME_H
#define TIME_H

#include "shared/iSingleton.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace time {

class TimeMgr: public Singleton<time> {
public:
    MAKE_SINGLETON(Time);
}

} //namespace time
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif