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


void SocketMgr::Start() {
    if(!m_running) {
        m_running = true;
        m_work = new boost::asio::io_service::work(m_service);
        m_runServerThread = boost::thread(boost::bind(&boost::asio::io_service::run, &m_service));
    }
}

void SocketMgr::Stop() {
    if(m_running) {
        vector<SocketServer*> servers;

        typedef std::map<string, SocketServer*> MapType;
        BOOST_FOREACH(MapType::value_type server, m_servers) {
            servers.push_back(server.second);
        }

        for (int i = 0; i < (int) servers.size(); i++) {
            DestroyServer(servers[i]);
        }

        delete m_work;
        m_service.stop();
        m_runServerThread.join();
        m_running = false;
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
} //namespace jRylServer