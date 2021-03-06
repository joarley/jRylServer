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
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/utils.h"
#include "shared/crypt/cCryptEngine.h"
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

SocketSession::SocketSession(io_service& ioservice) : m_Socket(ioservice), m_processingBarrier(2) {
    m_isWriting = false;
}

tcp::socket& SocketSession::Socket() {
    return m_Socket;
}

bool SocketSession::Start(SocketServer& server) {
    m_PacketProcessThread = boost::thread(&SocketSession::hPacketProcess, this);
    m_processingBarrier.wait();

    m_Server = &server;
    m_Socket.set_option(boost::asio::ip::tcp::no_delay(true));
    m_Connected = true;
    
    Buffer_ptr buff(new Buffer());
	m_Socket.async_receive(buffer(buff->Data(), buff->MaxLength()),
      boost::bind(&SocketSession::hRead, this,
      buff,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));

    return true;
}

bool SocketSession::Start() {
    m_PacketProcessThread = boost::thread(&SocketSession::hPacketProcess, this);
    m_processingBarrier.wait();

    m_Server = NULL;
	boost::system::error_code ec;
    m_Socket.set_option(boost::asio::ip::tcp::no_delay(true), ec);
	
    m_Connected = true;
    
    Buffer_ptr buff(new Buffer());
	m_Socket.async_read_some(buffer(buff->Data(), buff->MaxLength()),
      boost::bind(&SocketSession::hRead, this,
      buff,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
    return true;
}

bool SocketSession::ConnectServer(string address, string port) {
    boost::system::error_code ec;
	tcp::resolver res(m_Socket.get_io_service());
    tcp::resolver::query qry(boost::asio::ip::tcp::v4(), address, port);
    tcp::resolver::iterator iteBegin = res.resolve(qry, ec);
    if (ec) {
        Logger::GetInstance().ShowError(ec.message().c_str());
        return false;
    }

	m_Socket.connect(iteBegin->endpoint(), ec);
    if(ec) {
        return false;
    }
    return true;
}

void SocketSession::hPacketProcess() {
    boost::unique_lock<boost::mutex> umu(m_ProcessingMutex);
    m_processingBarrier.wait();
    while (m_Connected) {                     
        m_codPacketProcess.wait(umu);
        while (!m_ReadQueue.empty() && m_Connected) {
            Buffer_ptr buff = m_ReadQueue.front();
            
            if (PacketProcessCallBack != NULL) {
                PacketProcessCallBack(shared_from_this(), buff);
            }

            m_ReadQueueMutex.lock();
            m_ReadQueue.pop();
            m_ReadQueueMutex.unlock();
        }
    }
    m_ReadQueueMutex.lock();
    m_ReadQueue = queue<Buffer_ptr>();
    m_ReadQueueMutex.unlock();
}

void SocketSession::tratar(Buffer_ptr buff) {
	crypt::CryptEngine::GetInstance().XorDecryptPacketHeader(buff->Data());
	crypt::CryptEngine::Cryptkey key;
    buff->GetPack(key, 4);
	uint16 packetSize = buff->Get<uint16>(2) & ~(PACKET_TYPE_COMPRESSED | PACKET_TYPE_NORMAL);
	crypt::CryptEngine::GetInstance().XorDecryptPacketBody(buff->Data(), packetSize, key);
	size_t bufferSize = buff->Length();
	Buffer_ptr b;
	if(packetSize < bufferSize) {
		b.reset(new Buffer(bufferSize - packetSize));
		b->AddBytes(buff->Data() + packetSize, bufferSize - packetSize, 0);
	}
	buff->SetLength(packetSize);
	m_ReadQueueMutex.lock();
    m_ReadQueue.push(buff);
    m_ReadQueueMutex.unlock();
	
	if(packetSize < bufferSize) {
		tratar(b);	
	}	
}

void SocketSession::hRead(Buffer_ptr buff, const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error && m_Connected) {
		buff->SetLength(bytes_transferred);
		tratar(buff);        

        buff.reset(new Buffer());
		m_Socket.async_receive(buffer(buff->Data(), buff->MaxLength()),
          boost::bind(&SocketSession::hRead, this,
          buff,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
        
        m_codPacketProcess.notify_all();
    } else {
        Stop();
    }
}

void SocketSession::SetPacketProcessCallBack(PacketCallback PktProcessCallBack) {
    PacketProcessCallBack = PktProcessCallBack;
}

void SocketSession::SendPacket(PacketBase& packet) {
    if (!m_Connected) {
        return;
    }
	Buffer_ptr buff = packet.GetProcessedBuffer();
	crypt::CryptEngine::Cryptkey key;
    buff->GetPack(key, 4);
	crypt::CryptEngine::GetInstance().XorCrypt(buff, key);
    m_isWritingMutex.lock();
    m_WriteQueueMutex.lock();
    
    m_WriteQueue.push(buff);
    if (!m_isWriting) {
		m_Socket.async_send(buffer(buff->Data(), buff->Length()),
          boost::bind(&SocketSession::hWrite, this, buff, boost::asio::placeholders::error));
        
        m_isWriting = true;
    }
    
    m_WriteQueueMutex.unlock();
    m_isWritingMutex.unlock();
}

void SocketSession::hWrite(Buffer_ptr buff, const boost::system::error_code& error) {
    if (!m_Connected) {
        return;
    }

    m_isWritingMutex.lock();
    m_WriteQueueMutex.lock();
    
    if (!error) {
        m_WriteQueue.pop();
        if (!m_WriteQueue.empty()) {
            Buffer_ptr buff = m_WriteQueue.front();
			m_Socket.async_send(buffer(buff->Data(), buff->Length()),
              boost::bind(&SocketSession::hWrite, this, buff,boost::asio::placeholders::error));
        } else {
            m_isWriting = false;
        }
    } else {
        m_WriteQueue = queue<Buffer_ptr>();
        Stop();
    }

    m_WriteQueueMutex.unlock();
    m_isWritingMutex.unlock();
}

void SocketSession::Stop() {
    m_stopMutex.lock();
    if (!m_Connected) {
        m_stopMutex.unlock();
        return;
    }
    m_Connected = false;
    m_stopMutex.unlock();

    boost::system::error_code ec;
    
    m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    m_Socket.close(ec);

    m_codPacketProcess.notify_all();
    m_PacketProcessThread.join();
    if(m_Server != NULL) {
		m_Server->ReleaseClient(shared_from_this());
	}
}

SocketServer* SocketSession::GetServer() {
    return m_Server;
}

bool SocketSession::Connected() const {
    return m_Connected;
}
} //namespace network
} //namespace common
} //namespace shared
} //namespace jRylServer