#include "stable.h"

#include "crc32.h"

namespace Dw
{
static unsigned int _crc32_table[256];
static bool _crc_table_initiliazed = false;

void crc32_init(CRC32State *state)
{
    if(!_crc_table_initiliazed)
    {
        int i,j;
        unsigned long crc;
        for (i = 0; i < 256; i++)
        {
            crc = i;
            for (j = 0; j < 8; j++)
            {
                if (crc & 1)
                {
                    crc = (crc >> 1) ^ 0xEDB88320;
                }
                else
                {
                    crc >>= 1;
                }
            }
            _crc32_table[i] = crc;
        }

        _crc_table_initiliazed = true;
    }

    state->ok = true;
    state->crc32result = 0xffffffff;
}

void crc32_update(CRC32State *state, const unsigned char *data, int len)
{
    if(!state->ok)
    {
        return;
    }

    unsigned int result = state->crc32result;

    assert(len >= 4);
    if (len < 4 )
    {
        state->ok = false;
        return;
    }

    for(int i=0; i<len; i++)
    {
        unsigned int tmp = data[i];
        result = (result >> 8) ^ _crc32_table[(result & 0xFF) ^ tmp];
    }

    state->ok = true;
    state->crc32result = ~result;
}

unsigned int crc32_final(CRC32State *state)
{
    if(state->ok)
    {
        return state->crc32result;
    }

    return 0;
}
}