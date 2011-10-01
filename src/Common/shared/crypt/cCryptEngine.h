#ifndef CRIPTENGINE_H
#define	CRIPTENGINE_H

#include <cstdlib>

#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/iPackable.h"
#include "shared/iSingleton.h"
#include "shared/crypt/cSeed.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace crypt {

class CryptEngine: public  Singleton<CryptEngine>{
public:
    MAKE_SINGLETON(CryptEngine);

    enum GGError {
        GGERRO_Sucess,
        GGERRO_ClientKey,
        GGERRO_Checksum = 4
    };

    typedef struct _CRYPTKEY : Packable {
        uint8 Key1;
        uint8 Key2;
        uint16 CodePage;
    
        _CRYPTKEY() {
            Key1 = rand() % 10;
            while (Key1 == (Key2 = rand() % 10));
            CodePage = 0x0001;
        }
    
        void Pack(Buffer_ptr buffer) {
            buffer->Add(Key1);
            buffer->Add(Key2);
            buffer->Add(CodePage);
        }
    
        void Unpack(Buffer_ptr buffer) {
            Key1 = buffer->Get<uint8 > ();
            Key2 = buffer->Get<uint8 > ();
            CodePage = buffer->Get<uint16 > ();
        }
    } Cryptkey;

    bool Start();
    void Stop();

    void SetGGCryptParams(byte* GGClientSeedKey, byte* GGServerSeedKey, byte* GGKey);

    Cryptkey* getKey();
    void XorCrypt(Buffer_ptr buffer, Cryptkey& key);
    void XorDecrypt(Buffer_ptr buffer);
    
    void GGCrypt(Buffer_ptr buffer);
    GGError GGDecrypt(Buffer_ptr buffer);

    void XorCryptPacketHeader(LPBYTE bytes);
    void XorCryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key);
    void XorDecryptPacketHeader(LPBYTE bytes);
    void XorDecryptPacketBody(LPBYTE bytes, size_t size, Cryptkey& key);
private:
    CryptEngine();

    byte m_GGClientSeedKey[16];
    byte m_GGServerSeedKey[16];
    byte* m_GGKey;
    SeedCipher::SeedParam m_GGServerSeedParam;
    SeedCipher::SeedParam m_GGClientSeedParam;

    
};
} //namespace crypt
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif	/* CRIPTENGINE_H */

