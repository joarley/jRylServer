#ifndef ISTARTUP_CLASS_H
#define ISTARTUP_CLASS_H
namespace jRylServer {
namespace common {
namespace shared {

class StartupClass {
    virtual bool LoadConfig() = 0;
    virtual int Start() = 0;
    virtual bool Stop() = 0;
};
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif