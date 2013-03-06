#include "stable.h"
#include <time.h>

#include "dwcore.h"

static bool __sranded = false;

void dwsrand()
{
    srand(GetTickCount());
    __sranded = true;
}

int dwRand()
{
    if( !__sranded )
    {
        dwsrand();
    }
    return rand();
}
