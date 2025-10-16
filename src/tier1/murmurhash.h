#ifndef src_tier1_murmurhash_h
#define src_tier1_murmurhash_h

#include <cstdint>

uint32_t MurmurHash2(const void *key, int len, uint32_t seed);
uint32_t MurmurHash2LowerCase(char *pString, uint32_t nSeed);

#endif