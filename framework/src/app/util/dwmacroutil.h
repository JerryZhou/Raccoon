#pragma once
//NB! DO NOT INCLUDE THIS IN ANY HEADER FILES
/// dummy dependency stuffs
//////////////////////////////////////////////////////////////////////////
#ifndef DW_CHECK
#define DW_CHECK(con) if (!(con)) {return;}
#define DW_CHECK_RET(con, ret) if(!(con)) {return (ret);}
#define DW_CHECK_ASSERT(con) if(!(con)) {assert(false); return;}
#define DW_CHECK_ASSERT_RET(con, ret) if(!(con)) {assert(false); return (ret);}
#endif
//////////////////////////////////////////////////////////////////////////
