#pragma once
#include "dwcore/dwstring.h"
#include "dwcore/dwbytearray.h"
#include "dwcore/dwmargins.h"
#include "dwcore/dwsize.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwrect.h"

//------------------------------------------------------------------------------
struct DW_GRAPHICS_EXPORT DwVertex {
    float x, y, z;
    int diffuse;
    float tu, tv;
};

//------------------------------------------------------------------------------
struct DW_GRAPHICS_EXPORT DwVector3 {
    float x, y, z;
    DwVector3(){}
    DwVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
};

//------------------------------------------------------------------------------
enum BlendMode
{
    //! Invalid mode indicator.
    BM_INVALID,
    //! Use normal blending mode.
    BM_NORMAL,
    //! Use blending mode suitable for textures with premultiplied colours.
    BM_RTT_PREMULTIPLIED
};

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwGraphicsObjectData
{
public:
    DwGraphicsObjectData();
    virtual ~DwGraphicsObjectData();
    
    void AddRef();
    void Release();
protected:
    mutable int m_ref;
};
#define _data_addRef(d) if(d) {d->AddRef();}
#define _data_release(d) if(d) { d->Release(); d = NULL; }

//------------------------------------------------------------------------------
template<typename T>
class DwGraphicsObjectDataPtr
{
public:
    DwGraphicsObjectDataPtr(T* d=NULL) : m_data(d){
        _data_addRef(m_data);
    }
    DwGraphicsObjectDataPtr(const DwGraphicsObjectDataPtr& other) : m_data(NULL){
        *this = other;
    }
    DwGraphicsObjectDataPtr& operator=(const DwGraphicsObjectDataPtr& other){
        if(m_data == other.m_data) return *this;
        _data_release(m_data);
        m_data = other.m_data;
        _data_addRef(m_data);
        return *this;
    }
    ~DwGraphicsObjectDataPtr(){
        _data_release(m_data);
    }
    operator bool()const
    {
        return m_data != NULL;
    }
    operator T*()const
    {
        return m_data;
    }
    T* operator->()const{
        return m_data;
    }
    T& operator*()const{
        return (*m_data);
    }

    T* data(){
        return m_data;
    }
    const T* data() const{
        return m_data;
    }
protected:
    T* m_data;
};

//Data Stuffs
//------------------------------------------------------------------------------
#define _declare_data(Class) \
    public:\
    friend class Class##Data;\
    inline Class##Data * data() { return (Class##Data*)(m_data); } \
    inline const Class##Data * data()const { return (Class##Data*)(m_data); }

#define _d_data(Class) \
    Class##Data *d = data()

#define _d_data_const(Class) \
    const Class##Data *d = data()