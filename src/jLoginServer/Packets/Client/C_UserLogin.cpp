#include "C_UserLogin.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/database/cDBMgr.h"

#include "../../cAccount.h"
#include "../../cLoginServer.h"
#include "../Server/S_UserLogin.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;
using namespace database;
using namespace soci;
using namespace std;

UserLogin::UserLogin(Buffer_ptr buff, Account* acc): PacketBase(buff) {
    if(!acc->isAuthenticated()) {
        acc->Close();
        return;
    }

    session* sql = DBMgr::GetInstance().CreateSession();
    if(sql == NULL) {
        
        return;
    }

    try {        
        rowset<row> sqlResult = (sql->prepare << "select cf.charinfoid, gd.guildid, gd.name guildname, gd.nation guildnation, pt.partyid,"
                                                 "       cf.slot, cf.name, cf.sex, cf.hair, cf.face, cf.class, cf.fame, cf.medal, cf.level"
                                                 "  from charinfo cf"
                                                 "           left join guild gd"
                                                 "             on cf.guildid = gd.guildid"
                                                 "           left join party pt"
                                                 "             on cf.partyid = pt.partyid"
                                                 "where accountid = :accountid"
                                                 "  and excluded = 0",
                                                 use(acc->GetId()));
        
        rowset<row>::iterator begin = sqlResult.begin();
        while(begin != sqlResult.end()) {
            Character* character = new Character();  
            Character::CharGeneralDetails* details = new Character::CharGeneralDetails;
            Character::CharEquipView* equipview = new Character::CharEquipView;
            Character::CharGuildView* guildview = new Character::CharGuildView;

            uint32 id = begin->get<uint32>(string("charinfoid"));
            int slot  = begin->get<uint8>(string("slot"));            

            details->Id = id;
            details->GuildId = begin->get<uint32>(string("guildid"));
            details->PartyId = begin->get<uint32>(string("partyid"));
            details->Name = begin->get<string>(string("name"));
            details->Sex = (Character::CharSex)begin->get<uint8>(string("sex"));
            details->Hair = begin->get<uint8>(string("hair"));
            details->Face = begin->get<uint8>(string("face"));
            details->Class = (Character::CharClass)begin->get<uint16>(string("class"));
            details->Fame = begin->get<uint32>(string("fame"));
            details->Medals = begin->get<uint32>(string("medal"));
            details->Level = begin->get<uint8>(string("level"));
            
            guildview->Name = begin->get<string>(string("guildname"));
            guildview->Nation = (Character::GuildNation)begin->get<uint8>(string("guildnation"));

            rowset<row> sqlEquipResult = (sql->prepare << "select equippos, prototypeid"
                                                          "  from charitem"
                                                          " where charinfoid = :charinfoid"
                                                          "   and equippos <> 0",
                                                          use(id));

            rowset<row>::iterator equipbegin = sqlEquipResult.begin();
            while(equipbegin != sqlEquipResult.end()) {
                equipview->SetEquipPrototypeId((Character::EquipPos)equipbegin->get<int>(string("equippos")), equipbegin->get<uint16>(string("prototypeid")));
            }
            character->SetId(id);
            character->SetSlot(slot);
            character->SetGeneralDetails(details);
            character->SetEquipView(equipview);
            character->SetGuildView(guildview);

            acc->SetCharacter(slot, character);

            begin++;
        }
    } catch(exception e) {
        DBMgr::GetInstance().FreeSession(sql);
        Logger::GetInstance().ShowError("%s\n", e.what());
        acc->Close();
        return;
    }
    DBMgr::GetInstance().FreeSession(sql);
    
    PacketServer::UserLogin ack(acc);
    acc->GetSocketSession()->SendPacket(ack.GetProcessedBuffer());
}

UserLogin::~UserLogin() {
    
}

}
}
}
