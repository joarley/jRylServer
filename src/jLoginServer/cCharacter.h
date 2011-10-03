#ifndef CHAR_H
#define	CHAR_H

#include <string>
#include <shared/typedef.h>
#include <shared/iPackable.h>
#include <shared/cBuffer.h>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class Account;

class Character {
public:
    enum CharSex {
        SEX_NotChosen,
        SEX_Male,
        SEX_Female
    };

    enum CharClass {
        CLASS_NotChosen,
        CLASS_Fighter,
        CLASS_Rogue,
        CLASS_Mage,
        CLASS_Acolyte,
        CLASS_Defender,
        CLASS_Warrior,
        CLASS_Assassin,
        CLASS_Archer,
        CLASS_Sorcerer,
        CLASS_Enchanter,
        CLASS_Priest,
        CLASS_Cleric,
        CLASS_Combatant = 17,
        CLASS_Officiator,
        CLASS_Templar,
        CLASS_Attacker,
        CLASS_Gunner,
        CLASS_Rune,
        CLASS_Life,
        CLASS_Shadow
    };
    
    enum CharRace {
        RACE_Human,
        RACE_Akhan
    };

    enum EquipPos {
        EP_NotEquiped,
        EP_Helm,
        EP_Shirt,
        EP_Tunic,
        EP_Armor,
        EP_Boot,
        EP_Glove,
        EP_Neck,
        EP_RingLeft,
        EP_RingRight,
        EP_PrimaryWeaponLeft,
        EP_SecondaryWeaponLeft,
        EP_PrimaryWeaponRight,
        EP_SecondaryWeaponRight,
        EP_Mount1,
        EP_Mount2
    };

    enum GuildNation {
        GN_Kartefant,
        GN_Merkhadian
    };

    typedef struct _CHAR_EQUIP_VIEW : Packable {
        uint16 Helm;
        uint16 Shirt;
        uint16 Tunic;
        uint16 Armor;
        uint16 Boot;
        uint16 Glove;
        uint16 Neck;
        uint16 RingLeft;
        uint16 RingRight;
        uint16 PrimaryWeaponLeft;
        uint16 SecondaryWeaponLeft;
        uint16 PrimaryWeaponRight;
        uint16 SecondaryWeaponRight;
        uint16 Mount1;
        uint16 Mount2;

        _CHAR_EQUIP_VIEW() {
            Helm = 0;
            Shirt = 0;
            Tunic = 0;
            Armor = 0;
            Boot = 0;
            Glove = 0;
            Neck = 0;
            RingLeft = 0;
            RingRight = 0;
            PrimaryWeaponLeft = 0;
            SecondaryWeaponLeft = 0;
            PrimaryWeaponRight = 0;
            SecondaryWeaponRight = 0;
            Mount1 = 0;
            Mount2 = 0;
        }

        bool SetEquipPrototypeId(EquipPos pos, uint16 protoid) {
            switch(pos) {
            case EP_Helm:
                Helm = protoid;
                return true;
            case EP_Shirt:
                Shirt = protoid;
                return true;
            case EP_Tunic:
                Tunic = protoid;
                return true;
            case EP_Armor:
                Armor = protoid;
                return true;
            case EP_Boot:
                Boot = protoid;
                return true;
            case EP_Glove:
                Glove = protoid;
                return true;
            case EP_Neck:
                Neck = protoid;
                return true;
            case EP_RingLeft:
                RingLeft = protoid;
                return true;
            case EP_RingRight:
                RingRight = protoid;
                return true;
            case EP_PrimaryWeaponLeft:
                PrimaryWeaponLeft = protoid;
                return true;
            case EP_SecondaryWeaponLeft:
                SecondaryWeaponLeft = protoid;
                return true;
            case EP_PrimaryWeaponRight:
                PrimaryWeaponRight = protoid;
                return true;
            case EP_SecondaryWeaponRight:
                SecondaryWeaponRight = protoid;
                return true;
            case EP_Mount1:
                Mount1 = protoid;
                return true;
            case EP_Mount2:
                Mount2 = protoid;
                return true;
            default:
                return false;
            }
        }

        void Pack(Buffer_ptr buffer) {
            buffer->Add(Helm).Add(Shirt).Add(Tunic).Add(Armor).Add(Boot).Add(Glove)
                  .Add(Neck).Add(RingLeft).Add(RingRight).Add(PrimaryWeaponLeft)
                  .Add(SecondaryWeaponLeft).Add(PrimaryWeaponRight).Add(SecondaryWeaponRight)
                  .Add(Mount1).Add(Mount2);
        }

        void Unpack(Buffer_ptr buffer) {
            Helm = buffer->Get<uint16 > ();
            Shirt = buffer->Get<uint16 > ();
            Tunic = buffer->Get<uint16 > ();
            Armor = buffer->Get<uint16 > ();
            Boot = buffer->Get<uint16 > ();
            Glove = buffer->Get<uint16 > ();
            Neck = buffer->Get<uint16 > ();
            RingLeft = buffer->Get<uint16 > ();
            RingRight = buffer->Get<uint16 > ();
            PrimaryWeaponLeft = buffer->Get<uint16 > ();
            SecondaryWeaponLeft = buffer->Get<uint16 > ();
            PrimaryWeaponRight = buffer->Get<uint16 > ();
            SecondaryWeaponRight = buffer->Get<uint16 > ();
            Mount1 = buffer->Get<uint16 > ();
            Mount2 = buffer->Get<uint16 > ();
        }
    } CharEquipView;

    typedef struct _CHAR_GENERAL_DETAILS : Packable {
        uint32 Id;
        string Name;
        CharSex Sex;
        uint8 Hair;
        uint8 Face;
        CharRace Race;
        CharClass Class;
        uint32 Fame;
        uint32 Medals;
        uint32 GuildId;
        uint32 PartyId;
        uint8 Level;
        
        _CHAR_GENERAL_DETAILS() {
            Id = 0;
            Name = "";
            Sex = SEX_NotChosen;
            Hair = 0;
            Face = 0;
            Race = RACE_Human;
            Class = CLASS_NotChosen;
            Fame = 0;
            Medals = 0;
            GuildId = 0;
            PartyId = 0;
            Level = 0;
        }
        
        void Pack(Buffer_ptr buffer) {
            buffer->Add(Id).AddStringSizeFixed(Name, 16).Add<uint8>(Sex).Add(Hair)
                    .Add(Face).Add<uint8>(Race).Add<uint16>(Class).Add(Fame).Add(Medals)
                    .Add(GuildId).Add(PartyId).Add(Level);
        }

        void Unpack(Buffer_ptr buffer) {
            Id = buffer->Get<uint32>();
            Name = buffer->GetStringSizeFixed<string>(16);
            Sex = (CharSex) buffer->Get<uint8>();
            Hair = buffer->Get<uint8>();
            Face = buffer->Get<uint8>();
            Race =(CharRace) buffer->Get<uint8>();
            Class = (CharClass) buffer->Get<uint16>();
            Fame = buffer->Get<uint32>();
            Medals = buffer->Get<uint32>();
            GuildId = buffer->Get<uint32>();
            PartyId = buffer->Get<uint32>();
            Level = buffer->Get<uint8>();
        }
    } CharGeneralDetails;

    typedef struct _CHAR_GUILD_VIEW : Packable {
        GuildNation Nation;
        string Name;

        _CHAR_GUILD_VIEW() {
            Nation = GN_Kartefant;
            Name = "";
        }

        void Pack(Buffer_ptr buffer) {
            buffer->Add<uint8>(Nation)
                   .AddStringSizeFixed(Name, 11)
                   .Add<int32>(0)
                   .Add<int8>(0);
        }

        void Unpack(Buffer_ptr buffer) {
            Nation = (GuildNation)buffer->Get<uint8>();
            Name = buffer->GetStringSizeFixed<string>(16);
            buffer->Get<uint32>();
            buffer->Get<uint8>();
        }
    } CharGuildView;

    Character();
    ~Character();

    CharGeneralDetails* GetGeneralDetails();
    CharEquipView* GetEquipView();
    uint32 GetId();
    int GetSlot();
    CharGuildView* GetGuildView();

    void SetGeneralDetails(CharGeneralDetails* cgd);
    void SetEquipView(CharEquipView* cev);
    void SetGuildView(CharGuildView* guildView);
    void SetId(uint32 id);
    void SetSlot(int slot);
private:
    uint32 m_id;
    int m_slot;
    CharGeneralDetails* m_Details;
    CharEquipView* m_ShortEquipList;
    CharGuildView* m_GuildDetails;
    Account* m_account;
};

}
}
#endif	/* CHAR_H */

