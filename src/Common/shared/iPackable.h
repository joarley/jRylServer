#ifndef PACKABLE_H
#define PACKABLE_H

#include <cstddef>

#include <boost/shared_ptr.hpp>
#include "shared/typedef.h"


namespace jRylServer {
namespace common {
namespace shared {
class Buffer;
typedef boost::shared_ptr<Buffer> Buffer_ptr;

class Packable {
public:
    virtual void Pack(Buffer_ptr buffer) = 0;
    virtual void Unpack(Buffer_ptr buffer) = 0;
};
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif