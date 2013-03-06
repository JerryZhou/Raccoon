#pragma once

#include <iterator>

#include "dwcore/dwallocator.h"
#include "dwdequelist.h"
#include "dwtypeinfo.h"
#include "dwcore/dwatomicint.h"

//////////////////////////////////////////////////////////////////////////
// DwArrayList
//////////////////////////////////////////////////////////////////////////

DW_CORE_EXPORT void _dw_arraylist_construct_hook();
DW_CORE_EXPORT void _dw_arraylist_destruct_hook();

template <typename T
,typename Alloc = DwAllocator<T>
,typename Algorithm = DwDequeList>
class DwArrayList
{
public:
    template <typename T, typename Ref, typename Ptr>
    class ArrayListIter : public Algorithm::Iterator
    {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef ArrayListIter<T, T&, T*> iterator;
        typedef ArrayListIter<T, const T&, const T*> const_iterator;
        typedef typename Algorithm::Iterator BaseIterator;
        typedef ArrayListIter<T, Ref, Ptr> self_type;

    public:
        //用于基类对象自动隐式转换为派生类对象，需被begin，end等成员函数使用
        inline ArrayListIter() : BaseIterator()
        {

        }

        inline ArrayListIter(const BaseIterator& other) : BaseIterator(other)
        {

        }

        inline bool operator==(const self_type& other) const
        {
            return equal(other);
        }

        inline bool operator!=(const self_type& other) const
        {
            return !equal(other);
        }

        inline bool operator<(const self_type& other) const
        {
            return less(other);
        }

        inline bool operator<=(const self_type& other) const
        {
            return !greater(other);
        }

        inline bool operator>(const self_type& other) const
        {
            return greater(other);
        }

        inline bool operator>=(const self_type& other) const
        {
            return !less(other);
        }

        inline difference_type operator-(const self_type& other) const
        {
            return distance(other);
        }

        inline self_type& operator++()
        {
            increase();
            return *this;
        }

        inline self_type operator++(int)
        {
            self_type temp = *this;
            this->increase();
            return temp;
        }

        inline self_type& operator--()
        {
            decrease();
            return *this;
        }

        inline self_type operator--(int)
        {
            self_type temp = *this;
            this->decrease();
            return temp;
        }

        inline self_type& operator+=(difference_type n)
        {
            this->advance(n);
            return *this;
        }

        inline self_type operator+(difference_type n) const
        {
            self_type temp = *this;
            temp.advance(n);
            return temp;
        }

        inline self_type& operator-=(difference_type n)
        {
            this->recede(n);
            return *this;
        }

        inline self_type operator-(difference_type n)
        {
            self_type temp = *this;
            temp.recede(n);
            return temp;
        }

        inline pointer operator->() const
        {
            return &(reinterpret_cast<Node*>(address())->value());
        }

        inline reference operator*() const
        {
            return reinterpret_cast<Node*>(address())->value();
        }

        inline reference operator[](int n) const
        {
            return *(*this + n);
        }
    };

public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Alloc allocator_type;
    typedef Algorithm algorithm_type;
    typedef ArrayListIter<value_type, reference, pointer> ArrayListIterator;
    typedef typename ArrayListIterator::iterator iterator;
    typedef typename ArrayListIterator::const_iterator const_iterator;
    typedef DwArrayList<T, Alloc, Algorithm> _self_type;

public:
    inline DwArrayList(const allocator_type& alloc = allocator_type())
        : m_allocator(alloc)
    {
        _dw_arraylist_construct_hook();

        m_pRef = new DwAtomicInt(1);
        m_pAlgorithm = new algorithm_type;
    }

    inline DwArrayList(const _self_type& other) : m_pRef(other.m_pRef), m_pAlgorithm(other.m_pAlgorithm)
    {
        _dw_arraylist_construct_hook();

        m_pRef->ref();
    }

    inline ~DwArrayList()
    {
        listClear();

        _dw_arraylist_destruct_hook();
    }

    inline _self_type& operator=(const _self_type& other)
    {
        if (m_pRef != other.m_pRef)
        {
            listClear();
            m_pRef = other.m_pRef;
            m_pRef->ref();
            m_pAlgorithm = other.m_pAlgorithm;
        }
        return *this;
    }

    //插入相关

    inline void push_back(const_reference value)
    {
        append(value);
    }

    inline void push_front(const_reference value)
    {
        prepend(value);
    }

    inline void pop_front()
    {
        if (!isEmpty())
        {
            detach();
            nodeDestruct(reinterpret_cast<Node*>(m_pAlgorithm->first().address()));
            m_pAlgorithm->removeFirst();
        }
    }

    inline void pop_back()
    {
        if (!isEmpty())
        {
            detach();
            nodeDestruct(reinterpret_cast<Node*>(m_pAlgorithm->last().address()));
            m_pAlgorithm->removeLast();
        }
    }

    inline void append(const_reference value)
    {
        detach();
        Node* curNode = reinterpret_cast<Node*>(m_pAlgorithm->append().address());
        nodeConstruct(curNode, value);
    }

    inline void prepend(const_reference value)
    {
        detach();
        Node* curNode = reinterpret_cast<Node*>(m_pAlgorithm->prepend().address());
        nodeConstruct(curNode, value);
    }

    inline iterator insert(int id, const_reference value)
    {
        return insert(createIterator(id), value);
    }

    inline iterator insert(const iterator& it, const_reference value)
    {
        detach();
        iterator newIt = m_pAlgorithm->insert(it);
        if(newIt != iterator())
        {
            Node* curNode = reinterpret_cast<Node*>(newIt.address());
            nodeConstruct(curNode, value);
        }
        return newIt;
    }

    inline DwArrayList& operator<<(const_reference value)
    {
        append(value);
        return *this;
    }

    //删除相关

    inline iterator erase(iterator it)
    {
        if (!isEmpty())
        {
            detach();
            nodeDestruct(reinterpret_cast<Node*>(it.address()));
            return m_pAlgorithm->remove(it);
        }
        else
        {
            return iterator();
        }
    }

    iterator erase(iterator from, iterator to)
    {
        if (!isEmpty())
        {
            DW_ASSERT(from.index() <= to.index());
            int eraseCount = to.index() - from.index();
            int fromID = from.index();
            for (int i = 0; i < eraseCount; ++i)
            {
                remove(fromID);
            }
            return begin() + fromID;
        }
        else
        {
            return iterator();
        }
    }

    void clear()
    {
        *this = DwArrayList<T, Alloc, Algorithm>(this->m_allocator);
    }

    inline void remove(int id)
    {
        DW_ASSERT(id >= 0 && id < static_cast<int>(count()));
        detach();
        erase(createIterator(id));
    }

    inline bool removeOne(const_reference value)
    {
        detach();
        iterator it = iteratorOf(value, begin());
        if (it != end())
        {
            erase(it);
            return true;
        }
        return false;
    }

    int removeAll(const_reference value)
    {
        detach();
        int count = 0;
        iterator it = iteratorOf(value, begin());
        while (it != end())
        {
            it = iteratorOf(value, erase(it));
            ++count;
        }
        return count;
    }

    //属性相关

    inline allocator_type get_allocator() const
    {
        return m_allocator;
    }

    inline const_iterator begin() const
    {
        return m_pAlgorithm->begin();
    }

    inline const_iterator end() const
    {
        return m_pAlgorithm->end();
    }

    inline iterator begin()
    {
        detach();
        return m_pAlgorithm->begin();
    }

    inline iterator end()
    {
        detach();
        return m_pAlgorithm->end();
    }

    inline reference front()
    {
        DW_ASSERT(!isEmpty());
        detach();
        return reinterpret_cast<Node *>(m_pAlgorithm->first().address())->value();
    }

    inline const_reference front() const
    {
        DW_ASSERT(!isEmpty());
        return reinterpret_cast<Node *>(m_pAlgorithm->first().address())->value();
    }

    inline reference back()
    {
        DW_ASSERT(!isEmpty());
        detach();
        return reinterpret_cast<Node *>(m_pAlgorithm->last().address())->value();
    }

    inline const_reference back() const
    {
        DW_ASSERT(!isEmpty());
        return reinterpret_cast<Node *>(m_pAlgorithm->last().address())->value();
    }

    inline bool isEmpty() const
    {
        return m_pAlgorithm->isEmpty();
    }

    inline size_type count() const
    {
        return m_pAlgorithm->count();
    }

    inline size_type size() const
    {
        return count();
    }

    inline reference at(int id)
    {
        DW_ASSERT(id >= 0 && id < static_cast<int>(count()));
        detach();
        return reinterpret_cast<Node*>(m_pAlgorithm->at(id).address())->value();
    }

    inline const_reference at(int id) const
    {
        DW_ASSERT(id >= 0 && id < static_cast<int>(count()));
        return reinterpret_cast<Node*>(m_pAlgorithm->at(id).address())->value();
    }

    inline reference operator[](int id)
    {
        return at(id);
    }

    inline const_reference operator[](int id) const
    {
        return at(id);
    }

    inline int indexOf(const_reference t, int fromID = 0) const
    {
        iterator it = iteratorOf(t, createIterator(fromID));
        if (it == end())
        {
            return -1;
        }
        return it.index();
    }

    inline iterator createIterator(int id) const
    {
        return m_pAlgorithm->createIterator(id);
    }

private:
    class Node
    {
    public:
        inline T& value()
        {
            return *reinterpret_cast<T*>(DwTypeInfo<T>::isLarge || DwTypeInfo<T>::isStatic ? m_val_or_ptr : this);
        }
        union
        {
            void* m_val_or_ptr;
            typename allocator_type::pointer m_objPtr;
        };
    };

    inline void nodeConstruct(Node *n, const_reference value)
    {
        //尚未异常处理
        DW_ASSERT(NULL != n);
        if (DwTypeInfo<T>::isLarge || DwTypeInfo<T>::isStatic)
        {
            n->m_objPtr = m_allocator.allocate(1);
            m_allocator.construct(n->m_objPtr, value);
        }
        else if (DwTypeInfo<T>::isComplex)
        {
            m_allocator.construct(reinterpret_cast<allocator_type::pointer>(n), value);
        }
        else
        {
            ::memcpy(n, &value, sizeof(T));
        }
    }

    inline void nodeDestruct(Node *n)
    {
        DW_ASSERT(NULL != n);
        if (DwTypeInfo<T>::isLarge || DwTypeInfo<T>::isStatic)
        {
            m_allocator.destroy(n->m_objPtr);
            m_allocator.deallocate(n->m_objPtr, 1);
        }
        else if (DwTypeInfo<T>::isComplex)
        {
            m_allocator.destroy(reinterpret_cast<allocator_type::pointer>(n));
        }
    }

    void listCopy(iterator from, iterator to)
    {
        DW_ASSERT(isEmpty());
        while (from != to)
        {
            append(*from);
            ++from;
        }
    }

    void listClear()
    {
        if(0 == m_pRef->deref())
        {
            if (isNeedDestroy())
            {
                iterator it = begin();
                while (it != end())
                {
                    nodeDestruct(reinterpret_cast<Node*>(it.address()));
                    ++it;
                }
            }
            delete m_pRef;
            delete m_pAlgorithm;
        }
    }

    iterator iteratorOf(const_reference t, iterator fromIt) const
    {
        //尚未做迭代器有效性判断
        iterator it = fromIt;
        while(it != end())
        {
            if (*it == t)
            {
                break;
            }
            ++it;
        }
        return it;
    }

    inline bool isNeedDestroy()
    {
        return (DwTypeInfo<T>::isLarge || DwTypeInfo<T>::isStatic || DwTypeInfo<T>::isComplex);
    }

    inline void detach()
    {
        //调用listClear过程中可能出现 *m_pRef 等于 0，此时调用begin时不应分离
        if (*m_pRef != 1 && *m_pRef != 0)
        {
            detach_helper();
        }
    }

    inline void detach_helper()
    {
        m_pRef->deref();
        DW_ASSERT(*m_pRef !=0 );
        m_pRef = new DwAtomicInt(1);
        algorithm_type* backup = m_pAlgorithm;
        m_pAlgorithm = new algorithm_type;
        listCopy(backup->begin(), backup->end());
    }

private:
    DwAtomicInt* m_pRef;
    algorithm_type* m_pAlgorithm;
    allocator_type m_allocator;
};
