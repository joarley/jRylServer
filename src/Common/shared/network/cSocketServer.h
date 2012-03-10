#ifndef SOCKET_SERVER
#define SOCKET_SERVER

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>

#include "shared/network/cSocketSession.h"
#include "shared/typedef.h"

#include <set>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {

typedef boost::function<bool(SocketSession_ptr)> SocketServerCallBack;
typedef boost::function<void(SocketSession_ptr)> SocketServerCloseCallBack;

class SocketServer {
public:
    SocketServer(io_service& IOService);
    bool Bind(string listenAdress, string listenPort);
    bool Start();
    void Stop();
    bool Started();
    void SetAcceptCallBack(SocketServerCallBack ServerCallBack);
	void SetCloseCallBack(SocketServerCloseCallBack ServerCallBack);
    void ReleaseClient(SocketSession_ptr client);
    void SetName(string name);
    string GetName();
protected:
    bool m_started;
    void hAccept(SocketSession_ptr newClient, const boost::system::error_code& error);
    SocketServerCallBack AcceptCallBack;
	SocketServerCloseCallBack CloseCallBack;
    io_service& m_IOService;
    tcp::acceptor m_Acceptor;
    set<SocketSession_ptr> m_ClientSessions;
    string m_name;
};

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer
#endif