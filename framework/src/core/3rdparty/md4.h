#pragma once

namespace Dw
{
const int KMD4ResultLen = 128/8;

struct MD4Context
{
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    unsigned char buffer[64];
    uint32_t block[KMD4ResultLen];
};

void MD4Init(struct MD4Context *ctx);
void MD4Update(struct MD4Context *ctx, const unsigned char *data, size_t size);
void MD4Final(struct MD4Context *ctx, unsigned char result[KMD4ResultLen]);
}