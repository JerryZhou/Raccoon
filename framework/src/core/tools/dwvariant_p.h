#pragma once

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARAINT_CONSTRUCTOR
#define DW_VARAINT_CONSTRUCTOR(RetType, enumIndex, v) \
	DwVariant::DwVariant(RetType v)\
	{ is_null = false; type = enumIndex; data.##v = v; }
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_PCONSTRUCTOR
#define DW_VARIANT_PCONSTRUCTOR(RetType, enumIndex, v) \
	DwVariant::DwVariant(const RetType& v)\
	{ is_null = false; type = enumIndex; data.##v = new RetType(v); }
#endif
//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_AS
#define DW_VARIANT_AS(RetType, enumIndex) \
	template<>\
	RetType DwVariant::as<RetType>(bool *ok) const { return to##enumIndex(ok); }
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_TO
#define DW_VARIANT_TO(RetType, enumIndex, v) \
	RetType DwVariant::to##enumIndex(bool *ok) const\
	{\
		if(type == enumIndex)\
		{\
			if(ok)\
			{\
				*ok = true;\
			}\
			return data.##v;\
		}\
		if(ok)\
		{\
			*ok =false;\
		}\
		return RetType();\
	}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_TO_WITH
#define DW_VARIANT_TO_WITH(RetType, enumIndex, v, defV) \
	RetType DwVariant::to##enumIndex(bool *ok) const\
	{\
		if(type == enumIndex)\
		{\
			if(ok)\
			{\
				*ok = true;\
			}\
			return data.##v;\
		}\
		if(ok)\
		{\
			*ok =false;\
		}\
		return defV;\
	}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_PTO
#define DW_VARIANT_PTO(RetType, enumIndex, v) \
	RetType DwVariant::to##enumIndex(bool *ok) const\
	{\
		if(type == enumIndex)\
		{\
			if(ok)\
			{\
				*ok = true;\
			}\
			return *data.##v;\
		}\
		if(ok)\
		{\
			*ok =false;\
		}\
		return RetType();\
	}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_PTO_WITH
#define DW_VARIANT_PTO_WITH(RetType, enumIndex, v, defV) \
	RetType DwVariant::to##enumIndex(bool *ok) const\
	{\
		if(type == enumIndex)\
		{\
			if(ok)\
			{\
				*ok = true;\
			}\
			return *data.##v;\
		}\
		if(ok)\
		{\
			*ok =false;\
		}\
		return defV;\
	}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_OPERATOR
#define DW_VARIANT_OPERATOR(RetType, enumIndex, v) \
	case enumIndex:\
		data.##v = other.data.##v;\
		break
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_POPERATOR
#define DW_VARIANT_POPERATOR(RetType, enumIndex, v) \
	case enumIndex:\
		if (data.##v && type == enumIndex)\
		{\
			*data.##v = *other.data.##v;\
		}\
		else\
		{\
			data.##v = new RetType(*other.data.##v);\
		}\
		break
#endif


//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_RELEASE
#define DW_VARIANT_RELEASE(RetType, enumIndex, v) \
	case enumIndex:\
		break
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef DW_VARIANT_PRELEASE
#define DW_VARIANT_PRELEASE(RetType, enumIndex, v) \
	case enumIndex:\
		if (data.##v)\
		{\
			delete data.##v;\
			data.##v = NULL;\
		}\
		break
#endif