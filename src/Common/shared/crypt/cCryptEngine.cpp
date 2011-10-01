#include "cCryptEngine.h"
#include "CryptEngine.tab"

#include "shared/cBuffer.h"
#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/crypt/cSeed.h"
#include "shared/cCrc32.h"
#include "shared/utils.h"
#include "shared/iSingleton.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace crypt {

static uint32 s_ClientKeyId = 0x56;
static uint32 s_ServerKeyId = 0x3D;

CryptEngine::CryptEngine() {
    unsigned char GGRylKey[] = {0xC6, 0xD0, 0xC5, 0xB6, 0xB0, 0xD4, 0xC0, 0xD3, 0xB0, 0xA1, 0xB5, 0xE5, 0x00};
    byte GGClientSeedKey[16] = {0xA3, 0xFE, 0x91, 0x8F, 0xEB, 0xE1, 0xDC, 0x5C, 0x41, 0xE0, 0x20, 0x40, 0xAE, 0xA6, 0xAB, 0xA7};
    byte GGServerSeedKey[16] = {0x1B, 0xC2, 0x0A, 0xCA, 0x43, 0xDC, 0x7D, 0x2F, 0xB3, 0xA9, 0xD0, 0x36, 0x4D, 0x98, 0x6A, 0xEE};

    for (int i = 0; GGRylKey[i]; i++) {
        GGClientSeedKey[i % 16] ^= GGRylKey[i];
        GGServerSeedKey[i % 16] ^= GGRylKey[i];
    }

    SeedCipher::EncRoundKey(m_GGClientSeedParam, GGClientSeedKey);
    SeedCipher::EncRoundKey(m_GGServerSeedParam, GGServerSeedKey);
}

void CryptEngine::XorCrypt(Buffer_ptr buffer, Cryptkey& key) {
    CryptPacketBody(buffer->Data(), buffer->Length(), key);
    CryptPacketHeader(buffer->Data());
}

void CryptEngine::XorDecrypt(Buffer_ptr buffer) {
    DecryptPacketHeader(buffer->Data());
    Cryptkey key;
    buffer->GetPack(key, 4);
    DecryptPacketBody(buffer->Data(), buffer->Length(), key);
}

void CryptEngine::CryptPacketHeader(LPBYTE bytes) {
    for (int i = 1; i < PACKET_HEADER_SIZE; i++) {
        bytes[i] = bytes[i] ^ BitFields[i - 1];
        bytes[i] = (bytes[i] << 1) | (bytes[i] >> 7);
    }
}

void CryptEngine::CryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key) {
    int pos1 = (key.CodePage * 10 + key.Key1) * 40;
    int pos2 = (key.CodePage * 10 + key.Key2) * 40;
    for (uint i = PACKET_HEADER_SIZE, j = 0; i < size; i++, j++) {
        bytes[i] = bytes[i] ^ BitFields[pos1 + j % 40] ^ BitFields[pos2 + j % 40];
    }
}

void CryptEngine::DecryptPacketHeader(LPBYTE bytes) {
    for (int i = 1; i < PACKET_HEADER_SIZE; i++) {
        bytes[i] = (char) (bytes[i] >> 1) | (char) (bytes[i] << 7);
        bytes[i] = bytes[i] ^ BitFields[i - 1];
    }
}

void CryptEngine::DecryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key) {
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