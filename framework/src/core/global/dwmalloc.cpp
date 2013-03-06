#include "stable.h"
#include "dwmalloc.h"

#ifndef DW_CORE_NO_MEMPOOL
#include "dwmempool.h"

#define malloc mp_alloc
#define free mp_free
#define realloc mp_realloc
#endif

void *dwMalloc( size_t size )
{
    return malloc(size);
}

void *dwRealloc( void *p, size_t size )
{
    return realloc(p, size);
}

void dwFree( void *p )
{
    free(p);
}
