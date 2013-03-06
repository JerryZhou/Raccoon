#include "stable.h"
#include "dwmathutil.h"


int DwMathUtil::random()
{
    return ::rand();
}

int DwMathUtil::random( int from, int to )
{
    if (from == to)
    {
        return from;
    }

    return from + random()%(abs(to-from));
}

float DwMathUtil::randomF( float from, float to )
{
    if (from == to)
    {
        return from;
    }

    return from + random()*(to-from)/RAND_MAX;
}