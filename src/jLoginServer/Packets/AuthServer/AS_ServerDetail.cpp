#include "AS_ServerDetail.h"

#include "shared/typedef.h"


namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {
using namespace common::shared;

ServerDetail::ServerDetail(Buffer_ptr buff): PacketBase(buff) {
    m_Group = m_Buffer->Get<uint8>();
    m_Name = m_Buffer->GetString<string>();
    m_Adress = m_Buffer->Get<uint32>();
}

ServerDetail::~ServerDetail() {
    
}

}
}
}
