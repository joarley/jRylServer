#ifndef SEED_H
#define SEED_H
#include "shared/typedef.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace crypt {

class SeedCipher {
public:
    typedef uint32 SeedParam[32];
    static void Decrypt(byte* data, SeedParam keyparam);
    static void Encrypt(byte* data, SeedParam keyparam);
    static void EncRoundKey(SeedParam keyparam, byte* key);
protected:
    SeedCipher(){}
    static inline uint32 XorSSBox(uint32 value);
    static inline void Round(uint32& value0, uint32& value1, uint32& value2, uint32& value3, SeedParam keyparam, int posparam);
    static inline void RoundKey0(SeedParam keyparam, int posparam, uint32& value0, uint32& value1, uint32 value2, uint32 value3, uint32 kc);
    static inline void RoundKey1(SeedParam keyparam, int posparam, uint32 value0, uint32 value1, uint32& value2, uint32& value3, uint32 kc);

};

} //namespace crypt
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif