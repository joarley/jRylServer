#include "cSocketSession.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"

#include <stddef.h>
#include <queue>

using namespace boost::asio::ip;
using namespace boost::asio;

namespace jRylServer {
namespace common {
namespace shared {
namespace network {

SocketSession::SocketSession(io_service& ioservice) : m_Socket(ioservice) {
    m_isWriting = false;
    m_isProcessing = false;
}

tcp::socket& SocketSession::Socket() {
    return m_Socket;
}

bool SocketSession::Start(SocketServer& server) {
    m_Server = &server;
    m_Socket.set_option(boost::asio::ip::tcp::no_delay(true));
    m_Connected = true;
    m_PacketProcessThread = boost::thread(&SocketSession::hPacketProcess, this);

    Buffer_ptr buff(new Buffer());

    m_Socket.async_receive(buffer(buff->Data(), buff->MaxLength()),
      boost::bind(&SocketSession::hRead, this,
      buff,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
    return true;
}

void SocketSession::hPacketProcess() {
    while (m_Connected) {
        while (!m_ReadQueue.empty() && m_Connected) {
            m_ReadQueueMutex.lock();
            Buffer_ptr buff = m_ReadQueue.front();
            m_ReadQueue.pop();
            m_ReadQueueMutex.unlock();
            if (PacketProcessCallBack != NULL) {
                PacketProcessCallBack(shared_from_this(), buff);
            }
        }
        if(!m_Connected) {
            break;
        }
        boost::unique_lock<boost::mutex> umu(m_ProcessingMutex);
        m_codPacketProcess.wait(umu);
    }
    m_ReadQueueMutex.lock();
    m_ReadQueue = queue<Buffer_ptr>();
    m_ReadQueueMutex.unlock();
}

void SocketSession::hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error && m_Connected) {
        if(m_ReadQueue.size() > 512) {
            Logger::GetInstance().ShowError("Maximum size of the read queue reached client address: %s", m_Socket.remote_endpoint().address().to_string().c_str());
        } else {
            buff->SetMaxLength(bytes_transferred);
            buff->SetLength(bytes_transferred);
            m_ReadQueueMutex.lock();
            m_ReadQueue.push(buff);
            m_ReadQueueMutex.unlock();
        }
        m_codPacketProcess.notify_all();
        
        buff.reset(new Buffer());

        m_Socket.async_receive(buffer(buff->Data(), buff->MaxLength()),
          boost::bind(&SocketSession::hRead, this,
          buff,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    } else {
        Stop();
    }
}

void SocketSession::SetPacketProcessCallBack(PacketCallBack PktProcessCallBack) {
    PacketProcessCallBack = PktProcessCallBack;
}

void SocketSession::Write(Buffer_ptr buff) {
    if (!m_Connected) {
        return;
    }
    m_isWritingMutex.lock();
    if (!m_isWriting && m_Connected) {
        m_Socket.async_send(buffer(buff->Data(), buff->Length()),
          boost::bind(&SocketSession::hWrite, this, buff, boost::asio::placeholders::error));
        m_isWriting = true;
    } else if (m_Connected) {
        m_WriteQueueMutex.lock();
        m_WriteQueue.push(buff);
        m_WriteQueueMutex.unlock();
    }
    m_isWritingMutex.unlock();
}

void SocketSession::hWrite(Buffer_ptr buff, const boost::system::error_code& error) {
    if (!error && m_Connected) {
        m_isWritingMutex.lock();
        if (!m_WriteQueue.empty()) {
            m_WriteQueueMutex.lock();
            Buffer_ptr b = m_WriteQueue.front();
            m_WriteQueue.pop();
            b.reset();
            if (m_WriteQueue.size() > 0) {
                Buffer_ptr buff = m_WriteQueue.front();
                m_Socket.async_send(buffer(buff->Data(), buff->Length()),
                  boost::bind(&SocketSession::hWrite, this, buff,boost::asio::placeholders::error));
            }
            m_WriteQueueMutex.unlock();
        } else {
            m_isWriting = false;
        }
        m_isWritingMutex.unlock();
    } else {
        m_WriteQueueMutex.lock();
        m_WriteQueue = queue<Buffer_ptr>();
        m_WriteQueueMutex.unlock();
        Stop();
    }
}

void SocketSession::Stop() {
    if (!m_Connected) {
        return;
    }
    boost::system::error_code ec;
    m_Connected = false;
    //m_Socket.cancel(ec);
    m_Socket.close(ec);
    m_codPacketProcess.notify_one();
    m_PacketProcessThread.join();
    m_Server->ReleaseClient(shared_from_this());
}

SocketServer& SocketSession::GetServer() {
    return *m_Server;
}

bool SocketSession::Connected() const {
    return m_Connected;
}
} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer