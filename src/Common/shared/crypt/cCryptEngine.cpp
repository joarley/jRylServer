#include "cCryptEngine.h"
#include "CryptEngine.tab"

#include "shared/cBuffer.h"
#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/crypt/cSeed.h"
#include "shared/cCrc32.h"
#include "shared/utils.h"
#include "shared/iSingleton.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace crypt {

static uint32 s_ClientKeyId = 0x56;
static uint32 s_ServerKeyId = 0x3D;

CryptEngine::CryptEngine() {
    m_GGKey = NULL;
}

bool CryptEngine::Start() {
    if(m_GGKey == NULL) {
        Logger::GetInstance().ShowError("[CryptEngine] GGKey not defined.\n");
        return false;
    }
    for (int i = 0; m_GGKey[i]; i++) {
        m_GGClientSeedKey[i % 16] ^= m_GGKey[i];
        m_GGServerSeedKey[i % 16] ^= m_GGKey[i];
    }

    SeedCipher::EncRoundKey(m_GGClientSeedParam, m_GGClientSeedKey);
    SeedCipher::EncRoundKey(m_GGServerSeedParam, m_GGServerSeedKey);

    return true;
}

void CryptEngine::Stop() {
    delete[] m_GGKey;
}

void CryptEngine::SetGGCryptParams(byte* GGClientSeedKey, byte* GGServerSeedKey, byte* GGKey) {
    memcpy(m_GGClientSeedKey, GGClientSeedKey, sizeof(m_GGClientSeedKey));
    memcpy(m_GGServerSeedKey, GGServerSeedKey, sizeof(m_GGServerSeedKey));
    m_GGKey = (byte*) malloc(strlen((char*)GGKey) + 1);
    strcpy((char*)m_GGKey, (char*)GGKey);
}

void CryptEngine::XorCrypt(Buffer_ptr buffer, Cryptkey& key) {
    XorCryptPacketBody(buffer->Data(), buffer->Length(), key);
    XorCryptPacketHeader(buffer->Data());
}

void CryptEngine::XorDecrypt(Buffer_ptr buffer) {
    XorDecryptPacketHeader(buffer->Data());
    Cryptkey key;
    buffer->GetPack(key, 4);
    XorDecryptPacketBody(buffer->Data(), buffer->Length(), key);
}

void CryptEngine::XorCryptPacketHeader(LPBYTE bytes) {
    for (int i = 1; i < PACKET_HEADER_SIZE; i++) {
        bytes[i] = bytes[i] ^ BitFields[i - 1];
        bytes[i] = (bytes[i] << 1) | (bytes[i] >> 7);
    }
}

void CryptEngine::XorCryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key) {
    int pos1 = (key.CodePage * 10 + key.Key1) * 40;
    int pos2 = (key.CodePage * 10 + key.Key2) * 40;
    for (uint i = PACKET_HEADER_SIZE, j = 0; i < size; i++, j++) {
        bytes[i] = bytes[i] ^ BitFields[pos1 + j % 40] ^ BitFields[pos2 + j % 40];
    }
}

void CryptEngine::XorDecryptPacketHeader(LPBYTE bytes) {
    for (int i = 1; i < PACKET_HEADER_SIZE; i++) {
        bytes[i] = (char) (bytes[i] >> 1) | (char) (bytes[i] << 7);
        bytes[i] = bytes[i] ^ BitFields[i - 1];
    }
}

void CryptEngine::XorDecryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key) {
    int pos1 = (key.CodePage * 10 + key.Key1) * 40;
    int pos2 = (key.CodePage * 10 + key.Key2) * 40;
    for (uint i = PACKET_HEADER_SIZE, j = 0; i < size; i++, j++) {
        bytes[i] = bytes[i] ^ BitFields[pos1 + j % 40] ^ BitFields[pos2 + j % 40];
    }
}

void CryptEngine::GGCrypt(Buffer_ptr buffer) {
    //byte* data = buffer->Data();
    //size_t dataLen = buffer->Length();
    //uint32 buffDataLen = dataLen;
    //if((dataLen % 16 + buffer->Length()) > buffer->MaxLength()) {
    //    buffer->SetMaxLength(dataLen % 16 + buffer->MaxLength());
    //} 
    //while (dataLen % 16 != 0) {
    //    buffer->Add((uint8)(rand() % 256), dataLen++);
    //}
    //if((buffer->Length() + 16) > buffer->MaxLength()) {
    //    buffer->SetMaxLength(buffer->Length() + 16);
    //}

    //buffer->Add((uint32)0, dataLen); //m_dwServerSequenceNumber
    //buffer->Add(Crc32::Calculate(data, buffer->Length()), dataLen + 4);
    //buffer->Add(buffDataLen, dataLen + 8);
    //buffer->Add(s_ServerKeyId, dataLen + 12);
    //
    //dataLen = buffer->Length();
    //for (int i = 0; i < dataLen; i += 16) {
    //    //SeedCipher::Encrypt(data + i, GGServerSeedParam);
    //    SeedCipher::Encrypt(data + i, GGClientSeedParam);
    //}
}

CryptEngine::GGError CryptEngine::GGDecrypt(Buffer_ptr buffer) {
    byte* data = buffer->Data();
    size_t dataLen = buffer->Length();
    
    for (size_t i = 0; i < dataLen; i += 16) {
        SeedCipher::Decrypt(data + i, m_GGClientSeedParam);
    }
    
    if(EndianChange(buffer->Get<uint32>((int)buffer->Length() - 4)) != s_ClientKeyId) {
        return GGERRO_ClientKey;
    }
    
    uint32 GGDataSize = EndianChange(buffer->Get<uint32>((int)buffer->Length() - 8));
    uint32 GGDataCRC = EndianChange(buffer->Get<uint32>((int)buffer->Length() - 12));
    if(Crc32::Calculate(data, dataLen - 12) != GGDataCRC) {
        return GGERRO_Checksum;
    }
    
    uint32 GGUseSeq = EndianChange(buffer->Get<uint32>((int)buffer->Length() - 16));
    
    buffer->SetLength(GGDataSize);
    buffer->SetMaxLength(GGDataSize);
    return GGERRO_Sucess;
}

} //namespace crypt
} //namespace shared
} //namespace common
} //namespace jRylServer