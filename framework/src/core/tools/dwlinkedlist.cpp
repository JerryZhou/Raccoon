#include "stable.h"

#include "dwlinkedlist.h"

static int _dw_linkedlist_couter = 0;
void _dw_linkedlist_construct_hook()
{
    _dw_linkedlist_couter++;
}

void _dw_linkedlist_destruct_hook()
{
    _dw_linkedlist_couter--;
}
