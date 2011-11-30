#ifndef PacketClient_CHARCREATE_H
#define	PacketClient_CHARCREATE_H

#include "../../../Common/shared/cPacketBase.h"
#include "../../../Common/shared/typedef.h"
#include "../../cAccount.h"
#include "../../cCharacter.h"

namespace jRylServer {
    namespace PacketServer {

        enum CREATE_CHAR_ERROR{    
            CREATE_CHAR_SUCESS
        };

        class CharCreate : public PacketBase {
        public:
            static const uint8 PacketID = 0x06;
            static const uint16 PacketLength = 93;
            CharCreate(Account *account, Character *character, bool error = false, CREATE_CHAR_ERROR errorCode = CREATE_CHAR_SUCESS);
            LPBYTE GetBodyBytes();
        
            uint32 AccountId;
            uint32 CharSlot;
            uint32 CharacterId;
            char CharacterName[16];
            uint8 CharacterSex;
            uint8 CharacterHair;
            uint8 CharacterFace;
            uint8 CharacterRace;
            uint16 CharacterClass;
            uint32 CharacterFame;
            uint32 CharacterMedals;
            uint32 CharacterGuildId;
            uint32 CharacterPartyId;
			uint8 CharacterLevel;
            uint16 CharacterHelm;
            uint16 CharacterShirt;
            uint16 CharacterTunic;
            uint16 CharacterArmor;
            uint16 CharacterBoot;
            uint16 CharacterGlove;
            uint16 CharacterNeck;
            uint16 CharacterRingLeft;
            uint16 CharacterRingRight;
            uint16 CharacterPrimaryWeaponLeft;
            uint16 CharacterSecondaryWeaponLeft;
            uint16 CharacterPrimaryWeaponRight;
            uint16 CharacterSecondaryWeaponRight;
            uint16 CharacterMount1;
            uint16 CharacterMount2;

            bool m_Error;
            CREATE_CHAR_ERROR m_ErrorCode;
        };
    }
}

#endif

