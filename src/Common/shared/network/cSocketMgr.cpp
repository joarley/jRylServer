#include "cSocketMgr.h"

#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "shared/network/cSocketServer.h"
#include "shared/cLogger.h"
#include "shared/iSingleton.h"

#include <string>
#include <map>

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {


void SocketMgr::RunServer() {
    while(m_running) {
        boost::system::error_code ec;
        m_service.run(ec);
        if(ec) {
            Logger::GetInstance().ShowError("while processing SocketServers tasks: ", ec.message().c_str());
        }
        boost::this_thread::sleep(boost::posix_time::millisec(1));
    }
}

void SocketMgr::Start() {
    m_running = true;
    m_runServerThread = boost::thread(boost::bind(&SocketMgr::RunServer, this));
}

void SocketMgr::Stop() {
    m_running = false;
    m_service.stop();
    m_runServerThread.join();

    vector<SocketServer*> servers;

    typedef std::map<string, SocketServer*> MapType;
    BOOST_FOREACH(MapType::value_type server, m_servers) {
        servers.push_back(server.second);
    }

    for (int i = 0; i < (int) servers.size(); i++) {
        DestroyServer(servers[i]);
    }
}

SocketServer* SocketMgr::CreateServer(string name) {
    if(name == "" || m_servers.find(name) != m_servers.end()) {
        return NULL;
    }
    SocketServer* server = new SocketServer(m_service);
    server->SetName(name);
    m_servers[name] = server;
    return server;
}

void SocketMgr::DestroyServer(string name) {
    if(name != "" && m_servers.find(name) != m_servers.end()) {
        DestroyServer(m_servers[name]);
    }
}

void SocketMgr::DestroyServer(SocketServer* server) {
    if(server) {
        server->Stop();
        m_servers.erase(server->GetName());
        delete server;
    }
}

SocketServer* SocketMgr::GetServer(string name) {
    if(m_servers.find(name) != m_servers.end()) {
        return m_servers[name];
    }
    return NULL;
}

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer=