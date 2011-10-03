#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {

class GameServer {
public:
    bool IsLoggedAccount(uint32 accid);
    void CloseAccount(uint32 accid);
};

}
}
#endif