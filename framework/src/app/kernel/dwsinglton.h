#pragma once

template<typename T>
class DwSinglton
{
public:
    /// constructor
    DwSinglton();
    /// deconstrutor
    virtual ~DwSinglton();

    // get the instance of T, just retreve instance
    static T* Instance();

private:
    static T* _instance;
};

//------------------------------------------------------------------------------
/**
*/
template<typename T>
T* DwSinglton<T>::_instance = NULL;

//------------------------------------------------------------------------------
/**
*/
template<typename T>
DwSinglton<T>::DwSinglton()
{
    DW_ASSERT(!_instance && "should not create no more instance");
    _instance = (T*)(this);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
DwSinglton<T>::~DwSinglton()
{
    _instance = NULL;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
T* DwSinglton<T>::Instance()
{
    DW_ASSERT(_instance && "should create a instance first");
    return DwSinglton<T>::_instance;
}