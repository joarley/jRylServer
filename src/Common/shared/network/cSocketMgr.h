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
    boost::asio::io_service::work* m_work;
    boost::thread m_runServerThread;
    map<string, SocketServer*> m_servers;
    bool m_running;
};

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer
#endif
