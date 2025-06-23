#ifndef KECCAK_H
#define KECCAK_H
#include <stddef.h>
void keccak_256(unsigned char* out, const unsigned char* in, size_t inlen);
#endif
