#ifndef SOCKET_MGR
#define SOCKET_MGR

#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

#include "shared/network/cSocketServer.h"
#include "shared/iSingleton.h"

#include <string>
#include <map>

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {

class SocketMgr:public  iSingleton<SocketMgr> {
    MAKE_SINGLETON(SocketMgr);
public:
    void Start();
    void Stop();
    SocketServer* CreateServer(string name);
    SocketServer* GetServer(string name);
    void DestroyServer(string name);
    void DestroyServer(SocketServer* server);
private:
    boost::asio::io_service m_service;
    map<string, SocketServer*> m_servers;
    bool m_running;
    void RunServer();
    boost::thread m_runServerThread;
};

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer
#endif
