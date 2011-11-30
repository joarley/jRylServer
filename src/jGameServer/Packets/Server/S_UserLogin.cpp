#include "S_UserLogin.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"

#include "../../cAccount.h"
#include "../../cLoginServer.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer {

using namespace common::shared;

UserLogin::UserLogin(Account* acc): PacketBase(PacketID, PacketLength) {
    m_Buffer->Add(acc->GetSessionId())
             .Add(acc->GetId());

    for(int i = 0; i < CHAR_SLOT_LENGTH; i++) {
        Character* character = acc->GetCharacter(i);
        if(character == NULL) {
            m_Buffer->AddPack(Character::CharGeneralDetails())
                     .AddPack(Character::CharEquipView());
        } else {
            m_Buffer->AddPack(*character->GetGeneralDetails())
                     .AddPack(*character->GetEquipView());
        }
    }

    for(int i = 0; i < CHAR_SLOT_LENGTH; i++) {
        Character* character = acc->GetCharacter(i);
        if(character == NULL) {
            m_Buffer->AddPack(Character::CharGuildView());
        } else {
            m_Buffer->AddPack(*character->GetGuildView());
        }
    }

    m_Buffer->Add<int32>(0); //Flag
    m_Buffer->Add<uint8>(acc->GetNation()); //Nation
    m_Buffer->Add<int32>(0); //Timer
    m_Buffer->Add<int32>(0); //PlayTimer
    m_Buffer->Add<int16>(0); //GMLevel
    m_Buffer->Add<int8>(0); //BillingType
    m_Buffer->Add<int8>(0); //BillingUser
    m_Buffer->Add<int8>(0); //LoginType
}

UserLogin::~UserLogin() {
    
}

}
}
}
