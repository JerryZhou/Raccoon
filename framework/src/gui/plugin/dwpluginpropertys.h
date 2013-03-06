#pragma once
#include "dwgui/dwpropertys.h"

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_DECLARE_PROPERTY(type) \
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
#define DW_PLUGIN_DECLARE_SUB_PROPERTY(type, baseType) \
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
#define DW_PLUGIN_IMPLEMENT_PROPERTY(type)\
    DW_GLOBAL_STATIC(DwPropertys, _dw_propertys_##type);\
    DwPropertys* type::propertys()\
    {\
        return _dw_propertys_##type();\
    }

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY_BEGIN DW_WIDGET_PROPERTY_BEGIN

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_ROOT_PROPERTY_BEGIN DW_WIDGET_ROOT_PROPERTY_BEGIN

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY DW_WIDGET_PROPERTY

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY_NC DW_WIDGET_PROPERTY_NC

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY_PORTED DW_WIDGET_PROPERTY_PORTED

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY_PORTED_NC DW_WIDGET_PROPERTY_PORTED_NC

//------------------------------------------------------------------------------
/**
*/
#define DW_PLUGIN_PROPERTY_END DW_WIDGET_PROPERTY_END