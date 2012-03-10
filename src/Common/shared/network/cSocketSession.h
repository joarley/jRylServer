#ifndef SOCKET_SESSION_H
#define SOCKET_SESSION_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>

#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/cPacketBase.h"

#include <stddef.h>
#include <queue>
#include <string>

using namespace boost::asio::ip;
using namespace boost::asio;
using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {

class SocketServer;
class SocketSession;

typedef boost::shared_ptr<SocketSession> SocketSession_ptr;
typedef boost::function<void (SocketSession_ptr, Buffer_ptr) > PacketCallback;

class SocketSession : public boost::enable_shared_from_this<SocketSession> {
public:
    SocketSession(io_service& ioservice);
    bool ConnectServer(string address, string port);
	void SendPacket(PacketBase& packet);
    void Stop();
    bool Start(SocketServer& server);
    bool Start();
    tcp::socket& Socket();
    SocketServer* GetServer();
    void SetPacketProcessCallBack(PacketCallback PktProcessCallBack);
    bool Connected() const;

	void tratar(Buffer_ptr buff);
protected:
    void hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t size);
    void hWrite(Buffer_ptr buff, const boost::system::error_code& error);
    void hPacketProcess();
    PacketCallback PacketProcessCallBack;

    bool m_Connected;
    tcp::socket m_Socket;
    SocketServer* m_Server;
    std::queue<Buffer_ptr> m_WriteQueue;
    std::queue<Buffer_ptr> m_ReadQueue;
    bool m_isWriting;
    boost::thread m_PacketProcessThread;
    boost::condition_variable m_codPacketProcess;
    boost::mutex m_isProcessingMutex;
    boost::mutex m_WriteQueueMutex;
    boost::mutex m_ReadQueueMutex;
    boost::mutex m_ProcessingMutex;
    boost::mutex m_isWritingMutex;
    boost::mutex m_stopMutex;
    boost::barrier m_processingBarrier;
};

} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer

#endif