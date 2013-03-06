#include "stable.h"

#include "dwset.h"

static int _dw_set_couter = 0;
void _dw_set_construct_hook()
{
    _dw_set_couter++;
}

void _dw_set_destruct_hook()
{
    _dw_set_couter--;
}
