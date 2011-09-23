#include "cSocketServer.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <boost/foreach.hpp>

#include "shared/network/cSocketSession.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

#include <set>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {

SocketServer::SocketServer(io_service& IOService) :
m_IOService(IOService), m_Acceptor(IOService) {
    AcceptCallBack = NULL;
}

bool SocketServer::Bind(string listenAdress, string listenPort) {
    boost::system::error_code ec;
    tcp::resolver res(m_IOService);
    tcp::resolver::query qry(boost::asio::ip::tcp::v4(), listenAdress, listenPort);
    tcp::resolver::iterator iteBegin = res.resolve(qry, ec);
    if (ec) {
        Logger::GetInstance().ShowError(ec.message().c_str());
        return false;
    }
    tcp::resolver::iterator iteEnd;
    m_Acceptor.open(iteBegin->endpoint().protocol(), ec);
    if (ec) {
        Logger::GetInstance().ShowError(ec.message().c_str());
        return false;
    }
    while (iteBegin != iteEnd) {
        m_Acceptor.bind(iteBegin->endpoint(), ec);
        if (ec) {
            Logger::GetInstance().ShowError(ec.message().c_str());
            return false;
        }
        iteBegin++;
    }
    m_Acceptor.set_option(tcp::acceptor::reuse_address(true), ec);
    if (ec) {
        Logger::GetInstance().ShowError(ec.message().c_str());
        return false;
    }
    return true;
}

bool SocketServer::Start() {
    boost::system::error_code error;
    m_Acceptor.listen(100, error);
    if (error) {
        Logger::GetInstance().ShowError(error.message().c_str());
        return false;
    }
    SocketSession_ptr newClient(new SocketSession(m_IOService));
    m_Acceptor.async_accept(newClient->Socket(),
      boost::bind(&SocketServer::hAccept, this, newClient, boost::asio::placeholders::error));
    m_started = true;
    return true;
}

void SocketServer::hAccept(SocketSession_ptr newClient, const boost::system::error_code& error) {
    if (error) {
        Logger::GetInstance().ShowError(error.message().c_str());
        return;
    }
    if (AcceptCallBack == NULL || AcceptCallBack(newClient)) {
        if (newClient->Start(*this)) {
            m_ClientSessions.insert(newClient);
            newClient.reset(new SocketSession(m_IOService));
        } else {
            newClient->Socket().close();
        }
    } else {
        newClient->Socket().close();
    }
    m_Acceptor.async_accept(newClient->Socket(),
      boost::bind(&SocketServer::hAccept, this, newClient, boost::asio::placeholders::error));
}

void SocketServer::ReleaseClient(SocketSession_ptr client) {
    m_ClientSessions.erase(client);
}

void SocketServer::Stop() {
    if(!m_started) {
        return;
    }
    m_started = false;
    m_Acceptor.cancel();
    m_Acceptor.close();
    std::vector<SocketSession_ptr> clients;

    BOOST_FOREACH(SocketSession_ptr client, m_ClientSessions) {
        clients.push_back(client);
    }
    for (int i = 0; i < (int) clients.size(); i++) {
        clients[i]->Stop();
    }
}

void SocketServer::SetAcceptCallBack(SocketServerCallBack AccCallBack) {
    AcceptCallBack = AccCallBack;
}

void SocketServer::SetName(string name) {
    m_name = name;
}

string SocketServer::GetName() {
    return m_name;
}

bool SocketServer::Started() {
    return m_started;
}

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer