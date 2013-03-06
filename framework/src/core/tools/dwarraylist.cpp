#include "stable.h"

#include "dwarraylist.h"

static int _dw_arraylist_counter = 0;
void _dw_arraylist_construct_hook()
{
    _dw_arraylist_counter++;
}

void _dw_arraylist_destruct_hook()
{
    _dw_arraylist_counter--;
}
