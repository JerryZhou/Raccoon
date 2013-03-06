#pragma once
#include "dwcore/dwmap.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwglobalstatic.h"
#include "dwcore/dwvariant.h"

//------------------------------------------------------------------------------
/**
*/
namespace Dw
{
enum AccessMode
{
    ReadAccess = 0x01,
    WriteAccess = 0x02,
    StrongAccess = ReadAccess | WriteAccess,
};
}// end of namespace dw

//------------------------------------------------------------------------------
/**
*/
class DW_GUI_EXPORT DwPropertyBase
{
public:
    DwPropertyBase(Dw::AccessMode mode = Dw::StrongAccess):m_accessMode(mode) { }
    virtual ~DwPropertyBase() {}

    virtual void setProperty(void* obj, const DwVariant& ) = 0;
    virtual DwVariant propertyOf(const void* obj) const = 0;

protected:
    Dw::AccessMode m_accessMode;
private:
};

//------------------------------------------------------------------------------
/**
*/
template<typename TValue>
class DwSetterDelegateBase
{
public:
    virtual void setter(void *, const TValue& ) = 0;
protected:
private:
};

//------------------------------------------------------------------------------
/**
*/
template<typename TValue>
class DwGetterDelegateBase
{
public:
    virtual TValue getter(const void *) const  = 0;
protected:
private:
};

//------------------------------------------------------------------------------
/**
*/
template<typename ObjClass,
         typename TValue, void (ObjClass::*set)(const TValue&)>
class DwSetterDelegate : public DwSetterDelegateBase<TValue>
{
public:
    virtual void setter(void *obj, const TValue& v)
    {
        (((ObjClass*)(obj))->*set)(v);
    }
};

//------------------------------------------------------------------------------
/**
*/
template<typename ObjClass,
         typename TValue, void (ObjClass::*set)(TValue)>
class DwSetterDelegateNC : public DwSetterDelegateBase<TValue>
{
public:
    virtual void setter(void *obj, const TValue& v)
    {
        (((ObjClass*)(obj))->*set)(const_cast<TValue&>(v));
    }
};

//------------------------------------------------------------------------------
/**
*/
template<typename ObjClass,
         typename TValue, TValue (ObjClass::*get)()const >
class DwGetterDelegate : public DwGetterDelegateBase<TValue>
{
public:
    virtual TValue getter(const void *obj) const
    {
        return (((const ObjClass*)(obj))->*get)();
    }
protected:
private:
};

//------------------------------------------------------------------------------
/**
*/
template<typename TValue>
class DwProperty : public DwPropertyBase
{
public:
    DwProperty(Dw::AccessMode mode = Dw::StrongAccess)
        : DwPropertyBase(mode)
    {
        m_setter = NULL;
        m_getter = NULL;
    }
    ~DwProperty()
    {
        if(m_setter) delete m_setter;
        if(m_getter) delete m_getter;
    }

    template<typename ObjClass, void (ObjClass::*set)(const TValue&),
             TValue (ObjClass::*get)()const >
    static inline DwProperty<TValue>* Create()
    {
        DwProperty<TValue>* p = new DwProperty<TValue>;
        p->m_setter = new DwSetterDelegate<ObjClass, TValue, set>();
        p->m_getter = new DwGetterDelegate<ObjClass, TValue, get>();
        return p;
    }

    template<typename ObjClass, void (ObjClass::*set)(TValue),
             TValue (ObjClass::*get)()const >
    static inline DwProperty<TValue>* CreateNC()
    {
        DwProperty<TValue>* p = new DwProperty<TValue>;
        p->m_setter = new DwSetterDelegateNC<ObjClass, TValue, set>();
        p->m_getter = new DwGetterDelegate<ObjClass, TValue, get>();
        return p;
    }

    virtual void setProperty(void* obj, const DwVariant& v)
    {
        DW_ASSERT(m_accessMode & Dw::WriteAccess);
        bool formatCheck = true;
        m_setter->setter(obj, v.as<TValue>(&formatCheck));
        DW_ASSERT(formatCheck && "MUST SET THE RIGHT TYPE WHEN CALL setProperty");
    }

    virtual DwVariant propertyOf(const void* obj) const
    {
        DW_ASSERT(m_accessMode & Dw::ReadAccess);
        return DwVariant(m_getter->getter(obj));
    }

protected:
    DwSetterDelegateBase<TValue>* m_setter;
    DwGetterDelegateBase<TValue>* m_getter;
};

//------------------------------------------------------------------------------
/**
*/
class DW_GUI_EXPORT DwPropertys
{
public:
    DwPropertys();
    ~DwPropertys();

    bool registerProperty(DwString id, DwPropertyBase* p);

    void addProperty(DwString id, DwPropertyBase* p);

    void setProperty(void* obj, DwString id, const DwVariant& v);

    DwVariant propertyOf(const void* obj, DwString id)const;

    bool hasProperty(DwString id) const;

    DwMap<DwString, DwPropertyBase* > m_propertys;
};

//------------------------------------------------------------------------------
/**
*/
#define DW_DECLARE_PROPERTY(type) \
public:\
	typedef type SELFCLASS; \
	static DwPropertys* propertys();\
protected:\
	DwMap<DwString, DwVariant> m_propertys;\
public:\
	void addProperty(DwString id, DwPropertyBase* p)\
	{\
        type::propertys()->addProperty(id, p);\
	}\
	virtual void setProperty(DwString id, const DwVariant& v)\
	{\
		this->delayInitProperty();\
		if(type::propertys()->hasProperty(id))\
		{\
			type::propertys()->setProperty(this, id, v);\
			return;\
		}\
		m_propertys[id] = v;\
	}\
	virtual DwVariant propertyOf(DwString id)const\
	{\
		const_cast<type*>(this)->delayInitProperty();\
		if(type::propertys()->hasProperty(id))\
		{\
			return type::propertys()->propertyOf(this, id);\
		}\
		return m_propertys[id];\
	}\
	virtual bool hasProperty(DwString id)const\
	{\
		const_cast<type*>(this)->delayInitProperty();\
		if(type::propertys()->hasProperty(id))\
		{\
			return true;\
		}\
		if(m_propertys.contains(id))\
		{\
			return true;\
		}\
		return false;\
	}\
private:

//------------------------------------------------------------------------------
/**
*/
#define DW_DECLARE_SUB_PROPERTY(type, baseType) \
public:\
    typedef type SELFCLASS; \
    typedef baseType PARETNCLASS; \
    static DwPropertys* propertys();\
public:\
    void addProperty(DwString id, DwPropertyBase* p)\
    {\
        type::propertys()->addProperty(id, p);\
    }\
    virtual void setProperty(DwString id, const DwVariant& v)\
    {\
        if(type::propertys()->hasProperty(id))\
        {\
            type::propertys()->setProperty(this, id, v);\
            return;\
        }\
        PARETNCLASS::setProperty(id, v);\
    }\
    virtual DwVariant propertyOf(DwString id)const\
    {\
        const_cast<type*>(this)->delayInitProperty();\
        if(type::propertys()->hasProperty(id))\
        {\
            return type::propertys()->propertyOf(this, id);\
        }\
        return PARETNCLASS::propertyOf(id);\
    }\
    virtual bool hasProperty(DwString id)const\
    {\
        const_cast<type*>(this)->delayInitProperty();\
        if(type::propertys()->hasProperty(id))\
        {\
            return true;\
        }\
        return PARETNCLASS::hasProperty(id);\
    }\
private:

//------------------------------------------------------------------------------
/**
*/
#define DW_IMPLEMENT_PROPERTY(type)\
	DW_GLOBAL_STATIC(DwPropertys, _dw_propertys_##type);\
	DwPropertys* type::propertys()\
	{\
		return _dw_propertys_##type();\
	}

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTYS_REGISTER(type)\
	static const bool _dw_register_propertys_##type = type::initProperty()

//------------------------------------------------------------------------------
/**
*/
#define DW_PROPERTY(type, name, valueType, set, get) \
	static const int _dw_reg##name = type::propertys()->registerProperty(#name, DwProperty<valueType>::Create<type, &type::set, &type::get>())

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY_BEGIN(type, basetype) \
public:\
    typedef type PROPERTYTYPE;\
	virtual bool delayInitProperty() { return type::initProperty(); }\
    static bool initProperty()\
    {\
        basetype::initProperty();\
        static bool _dw_is_init = false;\
        if(_dw_is_init) return _dw_is_init

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_ROOT_PROPERTY_BEGIN(type) \
public:\
    typedef type PROPERTYTYPE;\
	virtual bool delayInitProperty() { return type::initProperty(); }\
    static bool initProperty()\
    {\
        static bool _dw_is_init = false;\
        if(_dw_is_init) return _dw_is_init


//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY(name, valueType, set, get) \
    static bool _dw_reg##name = false;\
    if(!_dw_reg##name)\
    {\
        _dw_reg##name = PROPERTYTYPE::propertys()->registerProperty(#name, DwProperty<valueType>::Create<PROPERTYTYPE, &PROPERTYTYPE::set, &PROPERTYTYPE::get>());\
    }

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY_NC(name, valueType, set, get) \
	static bool _dw_reg##name = false;\
	if(!_dw_reg##name)\
	{\
		_dw_reg##name = PROPERTYTYPE::propertys()->registerProperty(#name, DwProperty<valueType>::CreateNC<PROPERTYTYPE, &PROPERTYTYPE::set, &PROPERTYTYPE::get>());\
	}

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY_PORTED(name, valueType, protedType, set, get) \
	static bool _dw_reg##name = false;\
	if(!_dw_reg##name)\
	{\
		_dw_reg##name = PROPERTYTYPE::propertys()->registerProperty(#name, DwProperty<valueType>::Create<protedType, &protedType::set, &protedType::get>());\
	}

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY_PORTED_NC(name, valueType, protedType, set, get) \
	static bool _dw_reg##name = false;\
	if(!_dw_reg##name)\
	{\
		_dw_reg##name = PROPERTYTYPE::propertys()->registerProperty(#name, DwProperty<valueType>::CreateNC<protedType, &protedType::set, &protedType::get>());\
	}

//------------------------------------------------------------------------------
/**
*/
#define DW_WIDGET_PROPERTY_END() \
        _dw_is_init = true;\
        return _dw_is_init;\
    }