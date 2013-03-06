#pragma once

namespace Dw
{
struct Sha1State
{
    uint32_t h0;
    uint32_t h1;
    uint32_t h2;
    uint32_t h3;
    uint32_t h4;

    uint64_t messageSize;
    unsigned char buffer[64];
};


typedef union
{
    uint8_t  bytes[64];
    uint32_t words[16];
} Sha1Chunk;

void Sha1InitState(Sha1State *state);
void Sha1Update(Sha1State *state, const unsigned char *data, int64_t len);
void Sha1FinalizeState(Sha1State *state);
void Sha1ToHash(Sha1State *state, unsigned char* buffer);
}