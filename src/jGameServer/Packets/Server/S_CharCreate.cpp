#include "S_CharCreate.h"

namespace jRylServer {
    namespace PacketServer {
        CharCreate::CharCreate(Account *account, Character *character, bool error = false, CREATE_CHAR_ERROR errorCode = CREATE_CHAR_SUCESS): PacketBase(PacketID, PacketLength) {
            if(!error) {
                AccountId = account->GetId();
                CharSlot = character->GetSlot();
                CharacterId = character->GetId();
                memcpy(CharacterName, character->GetDetails().Name, sizeof(CharacterName));
                CharacterSex = character->GetDetails().Sex;
                CharacterHair = character->GetDetails().Face;
                CharacterFace = character->GetDetails().Fame;
                CharacterRace = character->GetDetails().Race;
                CharacterClass = character->GetDetails().Class;
                CharacterFame = character->GetDetails().Face;
                CharacterMedals = character->GetDetails().Medals;
                CharacterGuildId = character->GetDetails().GuildId;
                CharacterPartyId = character->GetDetails().PartyId;
                CharacterLevel = character->GetDetails().Level;
                CharacterHelm = character->GetShortEquipList().Helm;
                CharacterShirt = character->GetShortEquipList().Tunic;
                CharacterTunic = character->GetShortEquipList().Tunic;
                CharacterArmor = character->GetShortEquipList().Armor;
                CharacterBoot = character->GetShortEquipList().Boot;
                CharacterGlove = character->GetShortEquipList().Glove;
                CharacterNeck = character->GetShortEquipList().Neck;
                CharacterRingLeft = character->GetShortEquipList().RingLeft;
                CharacterRingRight = character->GetShortEquipList().RingRight;
                CharacterPrimaryWeaponLeft = character->GetShortEquipList().PrimaryWeaponLeft;
                CharacterSecondaryWeaponLeft = character->GetShortEquipList().PrimaryWeaponRight;
                CharacterPrimaryWeaponRight = character->GetShortEquipList().SecondaryWeaponLeft;
                CharacterSecondaryWeaponRight = character->GetShortEquipList().SecondaryWeaponRight;
                CharacterMount1 = character->GetShortEquipList().Mount1;
                CharacterMount2 = character->GetShortEquipList().Mount2;
            } else {
                m_ErrorCode = errorCode;
            }
            m_Error = error;
        }
        LPBYTE CharCreate::GetBodyBytes() {
            if(m_Error) {
                ClearBufferBody();
                m_Status = 0;
                AddToBody((uint32)m_ErrorCode);
                m_BodyOffset = PacketLength - HEADER_SIZE;
            }
        }
    }
}