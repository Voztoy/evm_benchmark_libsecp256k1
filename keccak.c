#include "keccak.h"
#include <stdint.h>
#include <string.h>

#define ROUNDS 24
#define KECCAKF_ROUNDS 24
#define SHA3_KECCAK_SPONGE_BIT 1600
#define KECCAK_RATE 1088
#define KECCAK_CAPACITY 512

static const uint64_t round_constants[24] = {
  0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
  0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
  0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
  0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
  0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
  0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
  0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
  0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

static void keccakf(uint64_t state[25]) {
  int i;
  for (i = 0; i < ROUNDS; i++) {
    uint64_t temp, bc[5];

    // Theta
    for (int j = 0; j < 5; j++)
      bc[j] = state[j] ^ state[j + 5] ^ state[j + 10] ^ state[j + 15] ^ state[j + 20];
    for (int j = 0; j < 5; j++) {
      temp = bc[(j + 4) % 5] ^ ((bc[(j + 1) % 5] << 1) | (bc[(j + 1) % 5] >> (64 - 1)));
      for (int k = 0; k < 25; k += 5)
        state[k + j] ^= temp;
    }

    // Rho and Pi
    uint64_t last = state[1];
    int j = 0;
    static const int rho[24] = {
      1,  3,  6, 10, 15, 21, 28, 36,
      45, 55, 2, 14, 27, 41, 56, 8,
      25, 43, 62, 18, 39, 61, 20, 44
    };
    static const int pi[24] = {
      10, 7, 11, 17, 18, 3, 5, 16,
      8, 21, 24, 4, 15, 23, 19, 13,
      12, 2, 20, 14, 22, 9, 6, 1
    };
    for (int i = 0; i < 24; i++) {
      int r = rho[i];
      uint64_t tmp = state[pi[i]];
      state[pi[i]] = (last << r) | (last >> (64 - r));
      last = tmp;
    }

    // Chi
    for (int j = 0; j < 25; j += 5) {
      uint64_t t[5];
      for (int i = 0; i < 5; i++) t[i] = state[j + i];
      for (int i = 0; i < 5; i++)
        state[j + i] ^= (~t[(i + 1) % 5]) & t[(i + 2) % 5];
    }

    // Iota
    state[0] ^= round_constants[i];
  }
}

void keccak_256(unsigned char* out, const unsigned char* in, size_t inlen) {
  uint64_t state[25];
  unsigned char temp[144];
  size_t i;

  memset(state, 0, sizeof(state));

  for (; inlen >= 136; inlen -= 136, in += 136) {
    for (i = 0; i < 136 / 8; i++)
      ((uint64_t*)state)[i] ^= ((uint64_t*)in)[i];
    keccakf(state);
  }

  memset(temp, 0, 144);
  memcpy(temp, in, inlen);
  temp[inlen] = 1;
  temp[135] |= 0x80;

  for (i = 0; i < 136 / 8; i++)
    ((uint64_t*)state)[i] ^= ((uint64_t*)temp)[i];
  keccakf(state);

  memcpy(out, state, 32);
}
