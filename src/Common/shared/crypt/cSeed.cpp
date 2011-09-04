#include "cSeed.h"
#include "seed.tab"

#include "shared/typedef.h"
#include "shared/utils.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace crypt {

static const uint32 KC0 = 0x9e3779b9;
static const uint32 KC1 = 0x3c6ef373;
static const uint32 KC2 = 0x78dde6e6;
static const uint32 KC3 = 0xf1bbcdcc;
static const uint32 KC4 = 0xe3779b99;
static const uint32 KC5 = 0xc6ef3733;
static const uint32 KC6 = 0x8dde6e67;
static const uint32 KC7 = 0x1bbcdccf;
static const uint32 KC8 = 0x3779b99e;
static const uint32 KC9 = 0x6ef3733c;
static const uint32 KC10 = 0xdde6e678;
static const uint32 KC11 = 0xbbcdccf1;
static const uint32 KC12 = 0x779b99e3;
static const uint32 KC13 = 0xef3733c6;
static const uint32 KC14 = 0xde6e678d;
static const uint32 KC15 = 0xbcdccf1b;

inline uint32 SeedCipher::XorSSBox(uint32 value) {
    return SS0[GetByte(value, 0)] ^
      SS1[GetByte(value, 1)] ^
      SS2[GetByte(value, 2)] ^
      SS3[GetByte(value, 3)];
}

inline void SeedCipher::Round(uint32& value0, uint32& value1, uint32& value2, uint32& value3, SeedParam keyparam, int posparam) {
    uint32 aux1, aux2, aux3;
    aux1 = keyparam[posparam + 1] ^ value1;
    aux2 = keyparam[posparam] ^ value0 ^ aux1;
    aux3 = XorSSBox(aux2);
    aux1 += aux3;
    aux2 = XorSSBox(aux1);
    aux3 += aux2;
    aux1 = XorSSBox(aux3);
    aux2 += aux1;
    value2 ^= aux1;
    value3 ^= aux2;
}

inline void SeedCipher::RoundKey0(SeedParam keyparam, int posparam, uint32& value0, uint32& value1, uint32 value2, uint32 value3, uint32 kc) {
    uint32 aux1, aux2;
    aux1 = value0;
    value0 = (value0 >> 8) ^ (value1 << 24);
    value1 = (value1 >> 8) ^ (aux1 << 24);
    aux1 = value0 + value2 - kc;
    aux2 = value1 + kc - value3;
    keyparam[posparam] = XorSSBox(aux1);
    keyparam[posparam + 1] = XorSSBox(aux2);
}

inline void SeedCipher::RoundKey1(SeedParam keyparam, int posparam, uint32 value0, uint32 value1, uint32& value2, uint32& value3, uint32 kc) {
    uint32 aux1, aux2;
    aux1 = value2;
    value2 = (value2 << 8) ^ (value3 >> 24);
    value3 = (value3 << 8) ^ (aux1 >> 24);
    aux1 = value0 + value2 - kc;
    aux2 = value1 + kc - value3;
    keyparam[posparam] = XorSSBox(aux1);
    keyparam[posparam + 1] = XorSSBox(aux2);
}

void SeedCipher::Decrypt(byte* data, SeedParam keyparam) {
    uint32 aux1, aux2, aux3, aux4;

    aux1 = EndianChange(((uint32 *) data)[0]);
    aux2 = EndianChange(((uint32 *) data)[1]);
    aux3 = EndianChange(((uint32 *) data)[2]);
    aux4 = EndianChange(((uint32 *) data)[3]);

    Round(aux3, aux4, aux1, aux2, keyparam, 30);
    Round(aux1, aux2, aux3, aux4, keyparam, 28);
    Round(aux3, aux4, aux1, aux2, keyparam, 26);
    Round(aux1, aux2, aux3, aux4, keyparam, 24);
    Round(aux3, aux4, aux1, aux2, keyparam, 22);
    Round(aux1, aux2, aux3, aux4, keyparam, 20);
    Round(aux3, aux4, aux1, aux2, keyparam, 18);
    Round(aux1, aux2, aux3, aux4, keyparam, 16);
    Round(aux3, aux4, aux1, aux2, keyparam, 14);
    Round(aux1, aux2, aux3, aux4, keyparam, 12);
    Round(aux3, aux4, aux1, aux2, keyparam, 10);
    Round(aux1, aux2, aux3, aux4, keyparam, 8);
    Round(aux3, aux4, aux1, aux2, keyparam, 6);
    Round(aux1, aux2, aux3, aux4, keyparam, 4);
    Round(aux3, aux4, aux1, aux2, keyparam, 2);
    Round(aux1, aux2, aux3, aux4, keyparam, 0);

    ((uint32 *) data)[0] = EndianChange(aux3);
    ((uint32 *) data)[1] = EndianChange(aux4);
    ((uint32 *) data)[2] = EndianChange(aux1);
    ((uint32 *) data)[3] = EndianChange(aux2);
}

void SeedCipher::Encrypt(byte* data, SeedParam keyparam) {
    uint32 aux1, aux2, aux3, aux4;

    aux1 = EndianChange(((uint32 *) data)[0]);
    aux2 = EndianChange(((uint32 *) data)[1]);
    aux3 = EndianChange(((uint32 *) data)[2]);
    aux4 = EndianChange(((uint32 *) data)[3]);

    Round(aux3, aux4, aux1, aux2, keyparam, 0);
    Round(aux1, aux2, aux3, aux4, keyparam, 2);
    Round(aux3, aux4, aux1, aux2, keyparam, 4);
    Round(aux1, aux2, aux3, aux4, keyparam, 6);
    Round(aux3, aux4, aux1, aux2, keyparam, 8);
    Round(aux1, aux2, aux3, aux4, keyparam, 10);
    Round(aux3, aux4, aux1, aux2, keyparam, 12);
    Round(aux1, aux2, aux3, aux4, keyparam, 14);
    Round(aux3, aux4, aux1, aux2, keyparam, 16);
    Round(aux1, aux2, aux3, aux4, keyparam, 18);
    Round(aux3, aux4, aux1, aux2, keyparam, 20);
    Round(aux1, aux2, aux3, aux4, keyparam, 22);
    Round(aux3, aux4, aux1, aux2, keyparam, 24);
    Round(aux1, aux2, aux3, aux4, keyparam, 26);
    Round(aux3, aux4, aux1, aux2, keyparam, 28);
    Round(aux1, aux2, aux3, aux4, keyparam, 30);

    ((uint32 *) data)[0] = EndianChange(aux3);
    ((uint32 *) data)[1] = EndianChange(aux4);
    ((uint32 *) data)[2] = EndianChange(aux1);
    ((uint32 *) data)[3] = EndianChange(aux2);
}

void SeedCipher::EncRoundKey(SeedParam keyparam, byte* key) {
    uint32 aux1, aux2, aux3, aux4;

    aux1 = EndianChange(((uint32 *) key)[0]);
    aux2 = EndianChange(((uint32 *) key)[1]);
    aux3 = EndianChange(((uint32 *) key)[2]);
    aux4 = EndianChange(((uint32 *) key)[3]);


    keyparam[0] = XorSSBox(aux1 + aux3 - KC0);
    keyparam[1] = XorSSBox(aux2 - aux4 + KC0);

    RoundKey0(keyparam, 2, aux1, aux2, aux3, aux4, KC1);
    RoundKey1(keyparam, 4, aux1, aux2, aux3, aux4, KC2);
    RoundKey0(keyparam, 6, aux1, aux2, aux3, aux4, KC3);
    RoundKey1(keyparam, 8, aux1, aux2, aux3, aux4, KC4);
    RoundKey0(keyparam, 10, aux1, aux2, aux3, aux4, KC5);
    RoundKey1(keyparam, 12, aux1, aux2, aux3, aux4, KC6);
    RoundKey0(keyparam, 14, aux1, aux2, aux3, aux4, KC7);
    RoundKey1(keyparam, 16, aux1, aux2, aux3, aux4, KC8);
    RoundKey0(keyparam, 18, aux1, aux2, aux3, aux4, KC9);
    RoundKey1(keyparam, 20, aux1, aux2, aux3, aux4, KC10);
    RoundKey0(keyparam, 22, aux1, aux2, aux3, aux4, KC11);
    RoundKey1(keyparam, 24, aux1, aux2, aux3, aux4, KC12);
    RoundKey0(keyparam, 26, aux1, aux2, aux3, aux4, KC13);
    RoundKey1(keyparam, 28, aux1, aux2, aux3, aux4, KC14);
    RoundKey0(keyparam, 30, aux1, aux2, aux3, aux4, KC15);
}

} //namespace crypt
} //namespace shared
} //namespace common
} //namespace jRylServer