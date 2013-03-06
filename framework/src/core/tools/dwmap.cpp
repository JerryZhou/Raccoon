#include "stable.h"

#include "dwmap.h"

static int _dw_map_couter = 0;
void _dw_map_construct_hook()
{
    _dw_map_couter++;
}

void _dw_map_destruct_hook()
{
    _dw_map_couter--;
}
