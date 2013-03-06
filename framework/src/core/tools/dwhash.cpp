#include "stable.h"

#include "dwhash.h"

static int _dw_hash_couter = 0;
void _dw_hash_construct_hook()
{
    _dw_hash_couter++;
}

void _dw_hash_destruct_hook()
{
    _dw_hash_couter--;
}

/*
    These functions are based on Peter J. Weinberger's hash function
    (from the Dragon Book). The constant 24 in the original function
    was replaced with 23 to produce fewer collisions on input such as
    "a", "aa", "aaa", "aaaa", ...
*/
static unsigned int _dw_hash(const unsigned char *p, int n)
{
    unsigned int h = 0;

    while (n--) {
        h = (h << 4) + *p++;
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

static unsigned int _dw_hash(const wchar_t *p, int n)
{
    unsigned int h = 0;

    while (n--) {
        h = (h << 4) + (*p++);
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

//------------------------------------------------------------------------------
namespace eastl
{
    size_t hash<DwString>::operator()(DwString val) const
    {
        return _dw_hash(val.constString(), val.length());
    }
    size_t hash<DwByteArray>::operator()(DwByteArray val) const
    {
        return _dw_hash((unsigned char*)(val.data()), val.size());
    }
    size_t hash<DwAtomicInt>::operator()(DwAtomicInt val) const
    {
        return (size_t)((int)(val));
    }
}