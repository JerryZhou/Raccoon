#include "stable.h"

#include "tools/dwlocks.h"
#include "dwmempool.h"

#define NUM_BIN		13
#define GAP_FILL	0xCDAB
#define MIN_CHUNK	(1024 * 128)
#define MAX_CHUNK	(1024 * 1024)

#define BINMAP_INDEX(h)			((h & 0xFFC0) >> 6)
#define SET_BINMAP_INDEX(h, i)	(h = (h & ~0xFFC0) | i << 6)
#define IS_FREE(h)				((BOOL)(h & 0x0001) != 0)

#define SET_FREE(h)				(h |= 0x0001)
#define SET_BUSY(h)				(h &= (~0x0001))

#define MALLOC(size)			malloc(size)
#define REMALLOC(p, newsize)	realloc(p, newsize)
#define FREE(p)					free(p)
#define MSIZE(p)				_msize(p)

#pragma pack(push, 2)
struct BLOCK
{
    uint16_t wHeadFlag;
    BLOCK *	pNext;
};
#pragma pack(pop)

struct BIN
{
    uint32_t uChunkSize;
    BLOCK *	pFirstBlock;
};

struct CHUNK_MAP
{
    void *		pAddr;
    uint32_t	uLen;
    BIN	*		pBin;
    uint32_t	uBlockSize;
};

struct MALLOC_STATE
{
    MALLOC_STATE()
    {
        memset(this, 0, sizeof(MALLOC_STATE));
        csLock = new DwCriticalSection();
    }
    ~MALLOC_STATE()
    {
        delete csLock;
        csLock = 0;
    }
    BIN					bin[NUM_BIN];
    CHUNK_MAP			chunkmap[1024];
    volatile uint32_t	uChunkCount;
    DwCriticalSection	*csLock;
};

static bool g_enableMempool = true;
static MALLOC_STATE *g_state = NULL;

static void mp_clear()
{
    delete g_state;
    g_state = NULL;
}

void mp_init()
{
    if(!g_state)
    {
        g_state = new MALLOC_STATE();
        atexit(mp_clear);
    }
}

void mp_enable(bool enable)
{
    mp_init();

    g_enableMempool = enable;
}

void *mp_alloc_chunk(size_t uSize)
{
    return VirtualAlloc(0, uSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void mp_error(const wchar_t* lpszFmt, ...)
{
    va_list args;
    va_start(args, lpszFmt);
    TCHAR szTemp[1024 * 4] = {0};
    _vsnwprintf_s(szTemp, _countof(szTemp) - 1, _TRUNCATE, lpszFmt, args);
    ::MessageBox(NULL, szTemp, L"mem pool error dectect!", MB_SERVICE_NOTIFICATION);
    ::DebugBreak();
    va_end(args);
}

void *mp_alloc(size_t uSize)
{
    if (!g_enableMempool)
    {
        return MALLOC(uSize);
    }

    mp_init();

    void *pRet = NULL;
    if (uSize == 0)
    {
        uSize = 1;
    }

    uint32_t uIndex;
    uint32_t uBlockSize;
    if (uSize <= 32)
    {
        uIndex = (uSize - 1) >> 3;
        uBlockSize = (uIndex + 1) << 3;
    }
    else if (uSize <= 96)
    {
        uIndex = 4 + ((uSize - 32 - 1) >> 4);
        uBlockSize = 32 + ((uIndex - 4 + 1) << 4);
    }
    else if (uSize <= 256)
    {
        uIndex = 4 + 4 + ((uSize - 96 - 1) >> 5);
        uBlockSize = 96 + ((uIndex - 4 - 4 + 1) << 5);
    }
    else
    {
        uIndex = -1;
        uBlockSize = -1;
    }

    if (uIndex != -1)
    {
        DwCSLocker locker(g_state->csLock);

        assert(uIndex < _countof(g_state->bin));

        if (!g_state->bin[uIndex].pFirstBlock)
        {
            assert(g_state->uChunkCount <= _countof(g_state->chunkmap));
            if (g_state->uChunkCount < _countof(g_state->chunkmap))
            {
                uint32_t uChunkSize = __max(MIN_CHUNK, __min(MAX_CHUNK, g_state->bin[uIndex].uChunkSize * 2));

                void *pChunk = mp_alloc_chunk(uChunkSize);
                memset(pChunk, 0, uChunkSize);

                BLOCK *pLast = NULL;
                for (uint8_t *p = (uint8_t*)pChunk + sizeof(uint16_t); p + sizeof(uint16_t) * 2 + uBlockSize < (uint8_t*)pChunk + uChunkSize; p += sizeof(uint16_t) * 2 + uBlockSize)
                {
                    if (!pLast)
                        pLast = g_state->bin[uIndex].pFirstBlock = (BLOCK*)p;
                    else
                        pLast = pLast->pNext = (BLOCK*)p;

                    pLast->pNext = NULL;

                    SET_BINMAP_INDEX(pLast->wHeadFlag, g_state->uChunkCount);
                    SET_FREE(pLast->wHeadFlag);
                    *(uint16_t*)((uint8_t*)pLast + sizeof(uint16_t) + uBlockSize) = GAP_FILL;
                }

                g_state->chunkmap[g_state->uChunkCount].pAddr = pChunk;
                g_state->chunkmap[g_state->uChunkCount].uLen = uChunkSize;
                g_state->chunkmap[g_state->uChunkCount].uBlockSize = uBlockSize;
                g_state->chunkmap[g_state->uChunkCount].pBin = &g_state->bin[uIndex];

                g_state->bin[uIndex].uChunkSize = uChunkSize;
                ::InterlockedIncrement((LONG*)&g_state->uChunkCount);
            }
        }

        if (g_state->bin[uIndex].pFirstBlock)
        {
            BLOCK *p = g_state->bin[uIndex].pFirstBlock;
            g_state->bin[uIndex].pFirstBlock = p->pNext;

            if (!IS_FREE(p->wHeadFlag) || *(uint16_t *)((const uint8_t*)p + sizeof(uint16_t) + uBlockSize) != GAP_FILL)
                mp_error(L"mp_alloc - detect block 0x%08X corrupt!", (const uint8_t*)p + sizeof(uint16_t));

            SET_BUSY(p->wHeadFlag);
            *(uint16_t*)((uint8_t*)p + sizeof(uint16_t) + uBlockSize) = GAP_FILL;
            pRet = (uint8_t*)p + sizeof(uint16_t);
        }
    }

    if (!pRet)
    {
        pRet = MALLOC(uSize);
    }

    return pRet;
}

BOOL mp_is_mempool_block(const void *p)
{
    BOOL bRet = FALSE;

    if (p)
    {
        if ((((uint32_t)p) & 0x3) == 0 && !IsBadReadPtr((uint8_t*)p - 4, 4))
        {
            const BLOCK *pBlock = (BLOCK*)((uint8_t*)p - sizeof(uint16_t));

            uint32_t uIndex = BINMAP_INDEX(pBlock->wHeadFlag);
            assert(uIndex < _countof(g_state->chunkmap));

            if (uIndex < g_state->uChunkCount && (uint8_t*)pBlock >= (uint8_t*)g_state->chunkmap[uIndex].pAddr && (uint8_t*)pBlock < (uint8_t*)g_state->chunkmap[uIndex].pAddr + g_state->chunkmap[uIndex].uLen)
                bRet = TRUE;
        }
    }
    return bRet;
}

void mp_free(void *p)
{
    if (p)
    {
        if (!g_enableMempool)
        {
            FREE(p);
        }
        else
        {
            mp_init();

            DwCSLocker locker(g_state->csLock);

            if (mp_is_mempool_block(p))
            {
                BLOCK *pBlock = (BLOCK*)((uint8_t*)p - sizeof(uint16_t));
                uint32_t uIndex = BINMAP_INDEX(pBlock->wHeadFlag);
                assert(uIndex < _countof(g_state->chunkmap));

                if ((uint8_t*)pBlock >= (uint8_t*)g_state->chunkmap[uIndex].pAddr && (uint8_t*)pBlock < (uint8_t*)g_state->chunkmap[uIndex].pAddr + g_state->chunkmap[uIndex].uLen)
                {
                    if (IS_FREE(pBlock->wHeadFlag))
                    {
                        mp_error(L"mp_free - repeat free block 0x%08X!", p);
                    }
                    else
                    {
                        uint32_t uBlockSize = g_state->chunkmap[uIndex].uBlockSize;
                        if (*(uint16_t *)((const uint8_t*)pBlock + sizeof(uint16_t) + uBlockSize) != GAP_FILL)
                            mp_error(L"mp_free - detect block 0x%08X corrupt!", p);

                        SET_FREE(pBlock->wHeadFlag);
                        *(uint16_t *)((uint8_t*)pBlock + sizeof(uint16_t) + uBlockSize) = GAP_FILL;

                        BIN *pBin = g_state->chunkmap[uIndex].pBin;
                        pBlock->pNext = pBin->pFirstBlock;
                        pBin->pFirstBlock = pBlock;
                    }
                }
                else
                {
                    mp_error(L"mp_free - block 0x%08X not in mem pool!", p);
                }
            }
            else
            {
                FREE(p);
            }
        }
    }
}

int mp_size(const void *p)
{
    unsigned int uSize = 0;
    if (p)
    {
        if (!g_enableMempool)
        {
            uSize = MSIZE((void*)p);
        }
        else
        {
            mp_init();

            DwCSLocker locker(g_state->csLock);

            if (mp_is_mempool_block(p))
            {
                const BLOCK *pBlock = (BLOCK*)((uint8_t*)p - sizeof(uint16_t));

                uint32_t uIndex = BINMAP_INDEX(pBlock->wHeadFlag);
                assert(uIndex < _countof(g_state->chunkmap));

                if (uIndex < g_state->uChunkCount && (uint8_t*)pBlock >= (uint8_t*)g_state->chunkmap[uIndex].pAddr && (uint8_t*)pBlock < (uint8_t*)g_state->chunkmap[uIndex].pAddr + g_state->chunkmap[uIndex].uLen)
                {
                    uSize = g_state->chunkmap[uIndex].uBlockSize;
                }
                else
                {
                    mp_error(L"mp_free - block 0x%08X not in mem pool!", p);
                    uSize = MSIZE((void*)p);
                }
            }
            else
            {
                uSize = MSIZE((void*)p);
            }
        }
    }

    return uSize;
}

void *mp_realloc(void *p, size_t uNewSize)
{
    if (!g_enableMempool)
    {
        return REMALLOC(p, uNewSize);
    }

    mp_init();

    void *pRet = NULL;
    if (p)
    {
        DwCSLocker locker(g_state->csLock);

        if (mp_is_mempool_block(p))
        {
            BLOCK *pBlock = (BLOCK*)((uint8_t*)p - sizeof(uint16_t));

            uint32_t uIndex = BINMAP_INDEX(pBlock->wHeadFlag);
            assert(uIndex < _countof(g_state->chunkmap));

            if (uIndex < g_state->uChunkCount && (uint8_t*)pBlock >= (uint8_t*)g_state->chunkmap[uIndex].pAddr && (uint8_t*)pBlock < (uint8_t*)g_state->chunkmap[uIndex].pAddr + g_state->chunkmap[uIndex].uLen)
            {
                uint32_t uBlockSize = g_state->chunkmap[uIndex].uBlockSize;
                if (uBlockSize >= uNewSize)
                {
                    if (IS_FREE(pBlock->wHeadFlag) || *(uint16_t *)((const uint8_t*)pBlock + sizeof(uint16_t) + uBlockSize) != GAP_FILL)
                        mp_error(L"mp_free - detect block 0x%08X corrupt!", p);

                    SET_BUSY(pBlock->wHeadFlag);
                    *(uint16_t *)((const uint8_t*)pBlock + sizeof(uint16_t) + uBlockSize) = GAP_FILL;

                    pRet = p;
                }
            }
            else
            {
                mp_error(L"mp_free - block 0x%08X not in mem pool!", p);
            }
        }
        else
        {
            pRet = REMALLOC(p, uNewSize);
        }
    }

    if (!pRet)
    {
        void *pNew = mp_alloc(uNewSize);

        if (p)
        {
            memcpy(pNew, p, mp_size(p));
            mp_free(p);
        }

        pRet = pNew;
    }

    return pRet;
}
