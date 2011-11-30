#include "C_UserLogin.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/database/cDBMgr.h"

#include "../../cAccount.h"
#include "../../cLoginServer.h"
#include "../../database.h"
#include "../Server/S_UserLogin.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;
using namespace database;
using namespace std;

UserLogin::UserLogin(Buffer_ptr buff, Account* acc): PacketBase(buff) {
    if(!acc->isAuthenticated()) {
        acc->Close();
        return;
    }

    RESULT_SET result;
    if(load_characters_view(acc->GetId(), result) == DS_ERROR) {
        return;
    }

    
    for(RESULT_SET::iterator it = result.begin(); it != result.end();it++) {
            Character* character = new Character();  
            Character::CharGeneralDetails* details = new Character::CharGeneralDetails;
            Character::CharEquipView* equipview = new Character::CharEquipView;
            Character::CharGuildView* guildview = new Character::CharGuildView;

            uint32 id = DB_CAST<uint32>((*it)["charinfoid"]);
            int slot  = DB_CAST<int32>((*it)["slot"]);            

            details->Id = id;
            details->GuildId = DB_CAST<uint32>((*it)["guildid"]);
            details->PartyId = DB_CAST<uint32>((*it)["partyid"]);
            details->Name = DB_CAST<string>((*it)["name"]);
            details->Sex = (Character::CharSex)DB_CAST<int8>((*it)["sex"]);
            details->Hair = DB_CAST<int8>((*it)["hair"]);
            details->Face = DB_CAST<int8>((*it)["face"]);
            details->Class = (Character::CharClass)DB_CAST<int16>((*it)["class"]);
            details->Fame = DB_CAST<uint32>((*it)["fame"]);
            details->Medals = DB_CAST<uint32>((*it)["medal"]);
            details->Level = DB_CAST<uint8>((*it)["level"]);
            
            guildview->Name = DB_CAST<string>((*it)["guildname"]);
            guildview->Nation = (Character::GuildNation)DB_CAST<int8>((*it)["guildnation"]);

            
            RESULT_SET equipresult;
            if(load_equips_view(details->Id, equipresult) == DS_ERROR) {
                delete guildview;
                delete equipview;
                delete details;
                delete character;
                continue;
            }
            
            for(RESULT_SET::iterator itequip = result.begin(); it != result.end();it++) {
                equipview->SetEquipPrototypeId((Character::EquipPos)DB_CAST<int8>((*it)["equippos"]), DB_CAST<uint16>((*it)["prototypeid"]));
            }
            character->SetId(id);
            character->SetSlot(slot);
            character->SetGeneralDetails(details);
            character->SetEquipView(equipview);
            character->SetGuildView(guildview);

            acc->SetCharacter(slot, character);

 
        }
    
    PacketServer::UserLogin ack(acc);
    acc->GetSocketSession()->SendPacket(ack.GetProcessedBuffer());
}

UserLogin::~UserLogin() {
    
}

}
}
}
