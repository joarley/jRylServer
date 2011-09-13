#ifndef DBMGR_H
#define DBMGR_H

#include "boost/thread/condition_variable.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"

#include "shared/database/iDBAdaptor.h"
#include "shared/database/iDBConnection.h"
#include "shared/database/cDBQuery.h"

#include <vector>
#include <queue>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

typedef boost::function<void (DBQuery*, DBResult*, int) > DBQueryCallBack;
typedef struct QUERY_CALLBACK_PAIR{
    DBQuery* query;
    DBQueryCallBack CallBack;
} QueryCallBackPair;

class DBMgr {
public:
	DBMgr(DBAdaptor* adaptor, int poolSize = 15);
    bool Start();
    void Stop();
	DBConnection* GetConnection();
	void FreeConnection(DBConnection* connection);
    void QueryExecuteAsync(DBQuery* query, DBQueryCallBack QueryCallBack = NULL);
    char* GetErrorDescription(int error);
    void ProcessQueryPairQueue(DBConnection* connection);
private:
    void hAsyncRequestThread();

    bool m_started;
    DBAdaptor* m_adaptor;
    std::vector<DBConnection*> m_connections;
    int m_poolSize;
    boost::thread m_asyncRequestThread;
    boost::condition_variable m_asyncRequestCod;
    boost::mutex m_asyncRequestMutex;
    std::queue<QueryCallBackPair*> m_queryPairQueue;
    boost::mutex m_queryPairQueueMutex;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif