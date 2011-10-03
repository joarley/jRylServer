#include <cstring>

#include "cCharacter.h"
#include <shared/typedef.h>


namespace jRylServer {
namespace jLoginServer {

Character::Character() {
}

Character::~Character() {

}

Character::CharGeneralDetails* Character::GetGeneralDetails() {
    return m_Details;
}

Character::CharEquipView* Character::GetEquipView() {
    return m_ShortEquipList;
}

uint32 Character::GetId() {
    return m_id;
}

int Character::GetSlot() {
    return m_slot;
}


void Character::SetGeneralDetails(CharGeneralDetails* cgd) {
    m_Details = cgd;
}

void Character::SetEquipView(CharEquipView* cev) {
    m_ShortEquipList = cev;
}

void Character::SetId(uint32 id) {
    m_id = id;
}

void Character::SetSlot(int slot) {
    m_slot = slot;
}

Character::CharGuildView* Character::GetGuildView() {
    return m_GuildDetails;
}

void Character::SetGuildView(CharGuildView* guildView) {
    m_GuildDetails = guildView;
}

}
}