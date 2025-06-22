// Source: Keccak / SHA3 reference implementation
#include <stdint.h>
#include <string.h>
#include "sha3.h"

#define SHA3_256_DIGEST_SIZE 32
#define SHA3_256_RATE 136

typedef struct {
    uint64_t state[25];
    uint8_t buffer[SHA3_256_RATE];
    size_t buffer_size;
} sha3_ctx;

static void keccakf(uint64_t st[25]);

void sha3_256_init(sha3_ctx* ctx) {
    memset(ctx, 0, sizeof(*ctx));
}

void sha3_256_update(sha3_ctx* ctx, const uint8_t* data, size_t len) {
    size_t i;
    for (i = 0; i < len; ++i) {
        ctx->buffer[ctx->buffer_size++] ^= data[i];
        if (ctx->buffer_size == SHA3_256_RATE) {
            memcpy(ctx->state, ctx->buffer, SHA3_256_RATE);
            keccakf(ctx->state);
            ctx->buffer_size = 0;
            memset(ctx->buffer, 0, SHA3_256_RATE);
        }
    }
}

void sha3_256_final(sha3_ctx* ctx, uint8_t* out) {
    ctx->buffer[ctx->buffer_size] ^= 0x06;
    ctx->buffer[SHA3_256_RATE - 1] ^= 0x80;
    memcpy(ctx->state, ctx->buffer, SHA3_256_RATE);
    keccakf(ctx->state);
    memcpy(out, ctx->state, SHA3_256_DIGEST_SIZE);
}

void sha3_256(uint8_t* out, const uint8_t* in, size_t inlen) {
    sha3_ctx ctx;
    sha3_256_init(&ctx);
    sha3_256_update(&ctx, in, inlen);
    sha3_256_final(&ctx, out);
}

// Keccak-f permutation implementation
#define ROL64(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))
static const uint64_t keccakf_rndc[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};
static const int keccakf_rotc[24] = {
     1,  3,  6, 10, 15, 21, 28, 36, 45, 55,  2, 14,
    27, 41, 56,  8, 25, 43, 62, 18, 39, 61, 20, 44
};
static const int keccakf_piln[24] = {
    10,  7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4,
    15, 23, 19, 13, 12, 2, 20, 14, 22,  9, 6,  1
};

static void keccakf(uint64_t st[25]) {
    int i, j, round;
    uint64_t t, bc[5];

    for (round = 0; round < 24; ++round) {
        for (i = 0; i < 5; ++i)
            bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];
        for (i = 0; i < 5; ++i) {
            t = bc[(i + 4) % 5] ^ ROL64(bc[(i + 1) % 5], 1);
            for (j = 0; j < 25; j += 5)
                st[j + i] ^= t;
        }

        t = st[1];
        for (i = 0; i < 24; ++i) {
            j = keccakf_piln[i];
            bc[0] = st[j];
            st[j] = ROL64(t, keccakf_rotc[i]);
            t = bc[0];
        }

        for (j = 0; j < 25; j += 5) {
            for (i = 0; i < 5; ++i)
                bc[i] = st[j + i];
            for (i = 0; i < 5; ++i)
                st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
        }

        st[0] ^= keccakf_rndc[round];
    }
}
