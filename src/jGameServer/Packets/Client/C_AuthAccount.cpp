#include "C_AuthAccount.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

#include "../../cLoginServer.h"
#include "../../cAccount.h"
#include "../../cCharacter.h"
#include "../Server/S_AuthAccount.h"
#include "../../database.h"

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
        return;
    }

    if(clientVersion != acc->GetServer()->GetVersion()) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_VersionError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(acc->GetServer()->GetCurrentUsersCount() >= acc->GetServer()->GetMaxUsers() ) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_ServerFull);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    RESULT result;
    DB_STATUS status = authenticate_account(user.c_str(), pass.c_str(), result);

    if(status == DS_ERROR) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_DBError);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(result.find("accountid") == result.end()) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_WrongLogin);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(DB_CAST<uint32>(result["banid"]) != 0) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_AlreadyLogged);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    if(acc->GetServer()->GetAccountById(DB_CAST<uint32>(result["accountid"])) != NULL) {
        PacketServer::AuthAccount authAccount(PacketServer::AuthAccount::AE_AlreadyLogged);
        acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
        return;
    }

    acc->SetId(DB_CAST<uint32>(result["accountid"]));
    acc->SetNation((Account::Nation)DB_CAST<int8>(result["nation"]));
    acc->GetServer()->LoginAccount(acc);

    PacketServer::AuthAccount authAccount(acc);
    
    acc->GetSocketSession()->SendPacket(authAccount.GetProcessedBuffer());
}

AuthAccount::~AuthAccount() {
    
}

}
}
}
