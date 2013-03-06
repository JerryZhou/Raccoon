#include "stable.h"
#include "dweventid.h"
//
////------------------------------------------------------------------------------
///**
//*/
//#ifdef _DEBUG
//DwEventId::DwEventId(DwEventId* parentId/* = NULL*/, const char* name/* = NULL*/)
//: m_parentId(parentId)
//, m_eventName(name)
//#else
//DwEventId::DwEventId(DwEventId* parentId/* = NULL*/)
//: m_parentId(parentId)
//#endif
//{
//	;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//DwEventId::~DwEventId()
//{
//	m_parentId = NULL;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool DwEventId::isA(const DwEventId* eventId) const
//{
//	DW_ASSERT(eventId);
//	if (eventId == this)
//	{
//		return true;
//	}
//
//	if (m_parentId)
//	{
//		return m_parentId->isA(eventId);
//	}
//	return false;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool DwEventId::isA(const DwEventId& id) const
//{
//	return isA(&id);
//}