#include "stable.h"

#include "dwvector.h"

static int _dw_vector_counter = 0;
void _dw_vector_construct_hook()
{
    _dw_vector_counter++;
}

void _dw_vector_destruct_hook()
{
    _dw_vector_counter--;
}
