#pragma once

namespace Dw
{
struct MD5Context
{
    uint32_t buf[4];
    uint32_t bytes[2];
    uint32_t in[16];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, uint8_t const *buf, unsigned len);
void MD5Final(struct MD5Context *context, uint8_t digest[16]);
void MD5Transform(uint32_t buf[4], uint32_t const in[16]);
}