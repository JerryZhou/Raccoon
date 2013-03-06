#pragma once

namespace Dw
{
struct CRC32State
{
    bool ok;
    unsigned int crc32result;
};

void crc32_init(CRC32State *state);
void crc32_update(CRC32State *state, const unsigned char *data, int len);
unsigned int crc32_final(CRC32State *state);
}
