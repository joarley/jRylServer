#include "C_AuthAccount.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/database/cDBMgr.h"
#include "shared/cLogger.h"

#include "../../cLoginServer.h"
#include "../../cAccount.h"
#include "../../cCharacter.h"
#include "../Server/S_AuthAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;

AuthAccount::AuthAccount(Buffer_ptr buff, Account* acc): PacketBase(buff) {
    if(acc->GetFistPing() == 0) {
        acc->Close();
        return;
    }

    if(acc->isAuthenticated()) {
        acc->Close();
        return;
    }

    string user = m_Buffer->GetStringSizeFixed<string>(16);
    string pass = m_Buffer->GetStringSizeFixed<string>(33);
    uint32 sessionID = m_Buffer->Get<uint32>();
    uint32 clientVersion = m_Buffer->Get<uint32>();    
    uint32 clientCheckSum =  m_Buffer->Get<uint32>();
    uint16 flag =  m_Buffer->Get<uint16>();

    if(clientCheckSum != acc->GetServer()->GetChecksum() && acc->GetServer()->GetChecksum() != 0x00000001) {
        Logger::GetInstance().ShowError("Incorrect Checksum Client 0x[%8X] Server 0x[%8X]\n", clientCheckSum, acc->GetServer()->GetChecksum());
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_ChecksumError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        acc->Close();
        return;
    }

    if(clientVersion != acc->GetServer()->GetVersion()) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_VersionError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        acc->Close();
        return;
    }

    if(acc->GetServer()->GetCurrentUsersCount() >= acc->GetServer()->GetMaxUsers() ) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_ServerFull);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        acc->Close();
        return;
    }

    soci::session* sql = database::DBMgr::GetInstance().CreateSession();
    if(sql == NULL) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_DBError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }
    
    soci::row sqlResult;
    try {
        *sql << "select acc.accountid, acc.nation, ban.accountid as banid"
                "  from account acc"
                "          left join banned_account ban"
                "            on acc.accountid = ban.accountid"
                " where user = :user"
                "   and pass = :pass",
                soci::into(sqlResult),
                soci::use(user, string("user")),
                soci::use(pass, string("pass"));
    } catch(exception e) {
        Logger::GetInstance().ShowError("%s\n", e.what());
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_DBError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(!sql->got_data()) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_WrongLogin);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(sqlResult.get_indicator(string("banid")) != soci::i_null) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_AccountBanned);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    uint32 accountId = sqlResult.get<int>(string("accountid"));

    if(acc->GetServer()->GetAccountById(accountId) != NULL) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_AlreadyLogged);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    acc->SetId(accountId);
    acc->SetNation((Account::Nation)sqlResult.get<uint8>(string("nation")));
    acc->GetServer()->LoginAccount(acc);    

    PacketServer::AuthAccount authAccount(acc);
    acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());

    database::DBMgr::GetInstance().FreeSession(sql);
}

AuthAccount::~AuthAccount() {
    
}

}
}
}
