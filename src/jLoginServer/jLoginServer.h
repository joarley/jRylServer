#ifndef JLOGINSERVER_H
#define	JLOGINSERVER_H
#include <boost/noncopyable.hpp>

#include "shared/iStartupClass.h"
#include "shared/typedef.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"

#include <vector>
#include <string>

using namespace std;

namespace jRylServer {
    using namespace common::shared;

    class jLoginServer :public StartupClass {
    public:
        jLoginServer(vector<string> params);
        bool LoadConfig();
        int Start();
        void Stop();
        virtual ~jLoginServer();        
    protected:
        uint32 m_Id;
    };
}

#endif	/* JLOGINSERVER_H */