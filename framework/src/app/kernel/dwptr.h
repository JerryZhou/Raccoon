#pragma once

//------------------------------------------------------------------------------
template<class TYPE>
class DwPtr
{
public:
    /// constructor
    DwPtr();
    /// construct from C++ pointer
    DwPtr(TYPE* p);
    /// construct from smart pointer
    DwPtr(const DwPtr<TYPE>& p);
    /// destructor
    ~DwPtr();
    /// assignment operator
    void operator=(const DwPtr<TYPE>& rhs);
    /// assignment operator
    void operator=(TYPE* rhs);
    /// equality operator
    bool operator==(const DwPtr<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const DwPtr<TYPE>& rhs) const;
    /// shortcut equality operator
    bool operator==(const TYPE* rhs) const;
    /// shortcut inequality operator
    bool operator!=(const TYPE* rhs) const;
    /// safe -> operator
    TYPE* operator->() const;
    /// safe dereference operator
    TYPE& operator*() const;
    /// safe pointer cast operator
    operator TYPE*() const;
    /// check if pointer is valid
    operator bool() const;
    /// type-safe downcast operator to other smart pointer
    template<class DERIVED> const DwPtr<DERIVED>& downcast() const;
    /// type-safe upcast operator to other smart pointer
    template<class BASE> const DwPtr<BASE>& upcast() const;
    /// unsafe(!) cast to anything, unless classes have no inheritance-relationship,
    /// call upcast/downcast instead, they are type-safe
    template<class OTHERTYPE> const DwPtr<OTHERTYPE>& cast() const;
    /// check if pointer is valid
    bool isvalid() const;
    /// return direct pointer (asserts if null pointer)
    TYPE* get() const;
    /// return direct pointer (returns null pointer)
    TYPE* get_unsafe() const;

private:
    TYPE* ptr;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::DwPtr() :
ptr(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::DwPtr(TYPE* p) :
ptr(p)
{
    if (0 != this->ptr)
    {
        this->ptr->addRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::DwPtr(const DwPtr<TYPE>& p) :
ptr(p.ptr)
{
    if (0 != this->ptr)
    {
        this->ptr->addRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::~DwPtr()
{
    if (0 != this->ptr)
    {
        this->ptr->release();
        this->ptr = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
DwPtr<TYPE>::operator=(const DwPtr<TYPE>& rhs)
{
    if (this->ptr != rhs.ptr)
    {
        if (this->ptr)
        {
            this->ptr->release();
        }
        this->ptr = rhs.ptr;
        if (this->ptr)
        {
            this->ptr->addRef();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
DwPtr<TYPE>::operator=(TYPE* rhs)
{
    if (this->ptr != rhs)
    {
        if (this->ptr)
        {
            this->ptr->release();
        }
        this->ptr = rhs;
        if (this->ptr)
        {
            this->ptr->addRef();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
DwPtr<TYPE>::operator==(const DwPtr<TYPE>& rhs) const
{
    return (this->ptr == rhs.ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
DwPtr<TYPE>::operator!=(const DwPtr<TYPE>& rhs) const
{
    return (this->ptr != rhs.ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
DwPtr<TYPE>::operator==(const TYPE* rhs) const
{
    return (this->ptr == rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
DwPtr<TYPE>::operator!=(const TYPE* rhs) const
{
    return (this->ptr != rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
DwPtr<TYPE>::operator->() const
{
    DW_ASSERT(this->ptr && "NULL pointer access in Ptr::operator->()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
DwPtr<TYPE>::operator*() const
{
    DW_ASSERT(this->ptr && "NULL pointer access in Ptr::operator*()!");
    return *this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::operator TYPE*() const
{
    DW_ASSERT(this->ptr && "NULL pointer access in Ptr::operator TYPE*()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwPtr<TYPE>::operator bool() const
{
    return this->isvalid();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class DERIVED> const DwPtr<DERIVED>&
DwPtr<TYPE>::downcast() const
{
#if _DEBUG
    // if DERIVED is not a derived class of TYPE, compiler complains here
    // compile-time inheritance-test
    const DERIVED *derived = static_cast<const DERIVED*>(this->ptr);
    DW_UNUSED(derived);
#endif
    return *reinterpret_cast<const DwPtr<DERIVED>*>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class BASE> const DwPtr<BASE>&
DwPtr<TYPE>::upcast() const
{
#if _DEBUG
    // if BASE is not a base-class of TYPE, compiler complains here
    // compile-time inheritance-test
    const BASE *base = this->ptr;
    DW_UNUSED(base);
#endif
    return *reinterpret_cast<const DwPtr<BASE>*>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
template<class OTHERTYPE> const DwPtr<OTHERTYPE>&
DwPtr<TYPE>::cast() const
{
    // note: this is an unsafe cast
    return *reinterpret_cast<const DwPtr<OTHERTYPE>*>(this);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
DwPtr<TYPE>::isvalid() const
{
    return (0 != this->ptr);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
DwPtr<TYPE>::get() const
{
    DW_ASSERT(this->ptr && "NULL pointer access in Ptr::get()!");
    return this->ptr;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE*
DwPtr<TYPE>::get_unsafe() const
{
    return this->ptr;
}
