#ifndef ISTARTUP_CLASS_H
#define ISTARTUP_CLASS_H
namespace jRylServer {
namespace common {
namespace shared {

class StartupClass {
public:
    virtual bool LoadConfig() = 0;
    virtual int Start() = 0;
    virtual void Stop() = 0;
    virtual ~StartupClass() {};
};
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif