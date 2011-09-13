#include "cDBMgr.h"

#include "boost/thread/condition_variable.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include "boost/bind.hpp"

#include "shared/database/iDBAdaptor.h"
#include "shared/database/iDBConnection.h"
#include "shared/database/cDBQuery.h"

#include <vector>
#include <queue>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

DBMgr::DBMgr(DBAdaptor* adaptor, int poolSize) {
    m_adaptor = adaptor;
    m_poolSize = poolSize;
}

bool DBMgr::Start() {
    for (int i = 0; i < m_poolSize; i++) {
        DBConnection* connection = m_adaptor->GetConnection();
        connection->SetIsTemp(false);
        m_connections.push_back(connection);
    }
    m_started = true;
    m_asyncRequestThread = boost::thread(boost::bind(&DBMgr::hAsyncRequestThread, this));
    return true;
}

void DBMgr::Stop() {
    m_started = false;
    m_asyncRequestCod.notify_all();
    m_asyncRequestThread.join();
    
    std::vector<DBConnection*>::iterator begin = m_connections.begin();
    std::vector<DBConnection*>::iterator end = m_connections.end();
    while (begin != end) {
        m_adaptor->FreeConnection(*begin);
    }
}

DBConnection* DBMgr::GetConnection() {
    DBConnection* connection = NULL;
    std::vector<DBConnection*>::iterator begin = m_connections.begin();
    std::vector<DBConnection*>::iterator end = m_connections.end();

    while (begin != end) {
        if (!(*begin)->InUse()) {
            connection = *begin;
        }
    }
    if (connection == NULL) {
        connection = m_adaptor->GetConnection();
        connection->SetIsTemp(true);
    }

    connection->SetInUse(true);
    return connection;
}

void DBMgr::FreeConnection(DBConnection* connection) {
    if (connection->IsTemp()) {
        m_adaptor->FreeConnection(connection);
    } else {
        connection->SetInUse(false);
    }
    connection = NULL;
}

void DBMgr::QueryExecuteAsync(DBQuery* query, DBQueryCallBack QueryCallBack) {
    if(!m_started) {
        return;
    }

    QueryCallBackPair* queryPair = new QueryCallBackPair;
    queryPair->query = query;
    queryPair->CallBack = QueryCallBack;

    m_queryPairQueueMutex.lock();
    m_queryPairQueue.push(queryPair);
    m_queryPairQueueMutex.unlock();
    
    m_asyncRequestCod.notify_all();
}

char* DBMgr::GetErrorDescription(int error) {
    char* description;
    DBConnection* connection = GetConnection();
    description = connection->GetErrorDescription(error);
    FreeConnection(connection);
    return description;
}

void DBMgr::hAsyncRequestThread() {
    
    while(m_started) {
        DBConnection* connection = GetConnection();
        while (!m_queryPairQueue.empty() && m_started) {
            m_queryPairQueueMutex.lock();
            QueryCallBackPair* queryPair = m_queryPairQueue.front();
            m_queryPairQueue.pop();
            m_queryPairQueueMutex.unlock();
            
            DBResult* result;
            result = connection->QueryExecute(queryPair->query);
            if(queryPair->CallBack != NULL) {
                queryPair->CallBack(queryPair->query, result, connection->GetLastErrorCode());
            }

            delete queryPair;
        }
        FreeConnection(connection);
        if(!m_started) {
            break;
        }

        
        
        boost::unique_lock<boost::mutex> unilock(m_asyncRequestMutex);
        m_asyncRequestCod.wait(unilock);
    }
    
    m_queryPairQueueMutex.lock();
    DBConnection* connection = GetConnection();
    
    while (!m_queryPairQueue.empty()) {
        QueryCallBackPair* queryPair = m_queryPairQueue.front();
        connection->QueryExecute(queryPair->query);
        m_queryPairQueue.pop();
        delete queryPair;
    }

    FreeConnection(connection);
    m_queryPairQueueMutex.unlock();
}

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer