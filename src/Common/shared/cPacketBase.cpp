#include "cPacketBase.h"

#include <cstring>

#include "shared/typedef.h"
#include "shared/crypt/cCryptEngine.h"
#include "shared/cBuffer.h"
#include "shared/stdDefs.h"
#include "minilzo/minilzo.h"

namespace jRylServer {
namespace common {
namespace shared {

using namespace crypt;

PacketBase::PacketBase(uint8 command, uint16 packetLength) {
    m_Startbit = PACKET_START_BIT;
    m_Command = command;
    m_Status = 0;
    m_Compressed = false;
    m_Buffer.reset(new Buffer(packetLength));
	m_Buffer->SetLength(packetLength);
    m_Buffer->SetWriteOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(uint8 command) {
    m_Startbit = PACKET_START_BIT;
    m_Command = command;
    m_Status = 0;
    m_Compressed = false;
    m_Buffer.reset(new Buffer());
    m_Buffer->SetWriteOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(Buffer_ptr buffer) {
    m_Buffer = buffer;
    m_Buffer->SetReaderOffset(0);

    m_Startbit = m_Buffer->Get<uint8 > ();
    m_Command = m_Buffer->Get<uint8 > ();
    m_Length = m_Buffer->Get<uint16 > ();
    m_Buffer->GetPack(m_Key);
    m_Status = m_Buffer->Get<uint32 > ();

    if (m_Length & PACKET_TYPE_COMPRESSED) {
        m_Compressed = true;
        LPBYTE bodyBytesDecompressed = new BYTE[(m_Buffer->Length() - PACKET_HEADER_SIZE) + (m_Buffer->Length() - PACKET_HEADER_SIZE) / 16 + 64 + 3];
        lzo_uint dstLen;
        lzo1x_decompress(m_Buffer->Data() + PACKET_HEADER_SIZE, m_Buffer->Length() - PACKET_HEADER_SIZE, bodyBytesDecompressed, &dstLen, NULL);
        m_Buffer->SetLength(dstLen);
        m_Buffer->AddBytes(bodyBytesDecompressed, dstLen, PACKET_HEADER_SIZE);
        delete[] bodyBytesDecompressed;
    } else {
        m_Compressed = false;
    }
}

Buffer_ptr PacketBase::GetProcessedBuffer() {
	m_Buffer->SetWriteOffset(PACKET_HEADER_SIZE);
	ProcessPacket();
	if (m_Compressed) {
        BYTE wrkmen[LZO1X_1_MEM_COMPRESS];
        LPBYTE dst = new BYTE[(m_Buffer->Length() - PACKET_HEADER_SIZE) + (m_Buffer->Length() - PACKET_HEADER_SIZE) / 16 + 64 + 3];
        lzo_uint dstLen;

        lzo1x_1_compress(m_Buffer->Data() + PACKET_HEADER_SIZE, m_Buffer->Length() - PACKET_HEADER_SIZE, dst, &dstLen, wrkmen);

        m_Buffer->SetLength(dstLen + PACKET_HEADER_SIZE);
        m_Buffer->AddBytes(dst, dstLen, PACKET_HEADER_SIZE);

        m_Length = (uint16) (m_Buffer->Length() | PACKET_TYPE_COMPRESSED);

        delete[] dst;
    } else {
        m_Length = (uint16) (m_Buffer->Length() | PACKET_TYPE_NORMAL);
    }

    m_Buffer->SetWriteOffset(0);
    m_Buffer->Add(m_Startbit).
      Add(m_Command).
      Add(m_Length).
      AddPack(m_Key).
      Add(m_Status);

    return m_Buffer;
}

} //namespace shared
} //namespace common
} //namespace jRylServer