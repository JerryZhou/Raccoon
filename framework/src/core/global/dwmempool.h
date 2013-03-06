#pragma once

#include "dwglobal.h"

DW_CORE_EXPORT void mp_enable(bool enable);
DW_CORE_EXPORT void *mp_alloc(size_t uSize);
DW_CORE_EXPORT void *mp_realloc(void *p, size_t uNewSize);
DW_CORE_EXPORT void	mp_free(void *p);
DW_CORE_EXPORT int	mp_size(const void *p);
