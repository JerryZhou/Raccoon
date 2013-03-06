
#include "dwdequelist.h"

//////////////////////////////////////////////////////////////////////////
//ChunkCache
//////////////////////////////////////////////////////////////////////////

class ChunkCache
{
private:
    friend class DwDequeList;

public:
    bool isHit(int id);
    void update(DwDequeList::Chunk* ptr, int minid, int maxid);

private:
    DwDequeList::Chunk* m_pChunk;
    int m_minID;
    int m_maxID;
};

inline bool ChunkCache::isHit(int id)
{
    return (m_pChunk != NULL && id >= m_minID && id <= m_maxID);
}

inline void ChunkCache::update(DwDequeList::Chunk* ptr, int minid, int maxid)
{
    m_pChunk = ptr;
    m_minID = minid;
    m_maxID = maxid;
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList::Iterator
//////////////////////////////////////////////////////////////////////////

void DwDequeList::Iterator::advance(DwDequeList::Iterator::difference_type n)
{
    if (n > 0 && (NULL != m_pChunk || !isEnd()))
    {
        int backcount = m_pChunk->backCount(m_slotID);
        while (n > backcount && NULL != m_pChunk->m_next)
        {
            m_pChunk = m_pChunk->m_next;
            n = n - backcount - 1;				//到达后继结点的首元素时应排除首元素本身
            m_id = m_id + backcount + 1;
            m_slotID = m_pChunk->m_begin;
            backcount = m_pChunk->backCount(m_slotID);
        }
        if (n <= backcount)						//最终n为结点内的偏移量，而m_id为结点内首元素索引
        {
            m_id += n;
            m_slotID = m_pChunk->incIndex(m_slotID, n);
        }
        else									//当偏移量n超过长度m_count时，将指向end
        {
            DW_ASSERT(NULL == m_pChunk->m_next);
            //注：不能直接加上m_pChunk->count()是为避免此时链表只有一个结点时m_id不是首元素索引
            m_id = m_id + m_pChunk->backCount(m_slotID) + 1;
            m_slotID = m_pChunk->m_end;
        }
    }
}

void DwDequeList::Iterator::recede(DwDequeList::Iterator::difference_type n)
{
    if (n > 0 && (NULL != m_pChunk || !isBegin()))
    {
        int frontcount = m_pChunk->frontCount(m_slotID);
        while (n > frontcount && NULL != m_pChunk->m_prev)
        {
            m_pChunk = m_pChunk->m_prev;
            n = n - frontcount - 1;				//到达前驱结点的首元素时应排除最后一个元素本身
            m_id = m_id - frontcount - 1;		//m_id为当前结点的尾元素索引
            m_slotID = m_pChunk->decIndex(m_pChunk->m_end);
            frontcount = m_pChunk->frontCount(m_slotID);
        }
        if (n <= frontcount)
        {
            m_id -= n;
            m_slotID = m_pChunk->decIndex(m_slotID, n);
        }
        else
        {
            DW_ASSERT(NULL == m_pChunk->m_prev);	//当n大于当前m_id之前的元素总数时，将指向begin
            m_id = 0;
            m_slotID = m_pChunk->m_begin;		//指向首元素
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//DwDequeList
//////////////////////////////////////////////////////////////////////////

DwDequeList::Chunk* DwDequeList::halveNode(Chunk* pChunk, int& offset, int offsetID)
{
    offsetID;
    DW_ASSERT(pChunk->full());
    int moveCount = 0;
    int midOffset = pChunk->count() / 2;
    int midID = pChunk->incIndex(pChunk->m_begin, midOffset);
    Chunk* newNode = allocate();
    if (offset <= pChunk->count() - offset)	//offset在前一半，则newNode放在pChunk前，此时不需修改offset
    {
        newNode->m_prev = pChunk->m_prev;
        newNode->m_next = pChunk;
        if(NULL != newNode->m_prev)
        {
            newNode->m_prev->m_next = newNode;
        }
        pChunk->m_prev = newNode;
        int newNodeID = newNode->m_begin;
        int pChunkID = pChunk->m_begin;
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;					//注：newNodeID不必循环自增，一定不会超过缓冲区边界
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == offsetID && newNodeID == offset);
        ++newNodeID;						//未自增前newNodeID为插入位置
        moveCount = midOffset - offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == midID && newNodeID == midOffset + 1);
        newNode->m_end = newNodeID;
        pChunk->m_begin = pChunkID;
        if (pChunk == m_head)
        {
            m_head = newNode;
        }
    }
    else									//offset在后一半，则newNode放在pChunk后，此时必须修改offset
    {
        newNode->m_prev = pChunk;
        newNode->m_next = pChunk->m_next;
        if(NULL != newNode->m_next)
        {
            newNode->m_next->m_prev = newNode;
        }
        pChunk->m_next = newNode;
        offset = offset - midOffset - 1;	//插入点在后一半时需额外处理offset
        int newNodeID = newNode->m_begin;
        int pChunkID = pChunk->incIndex(midID);
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == offsetID && newNodeID == offset);
        ++newNodeID;
        moveCount = pChunk->count() - (midOffset + 1 + offset);
        for (int i = 0; i < moveCount; ++i)
        {
            newNode->m_buf[newNodeID] = pChunk->m_buf[pChunkID];
            ++newNodeID;
            pChunkID = pChunk->incIndex(pChunkID);
        }
        DW_ASSERT(pChunkID == pChunk->m_end && newNodeID == pChunk->count() - midOffset);
        pChunk->m_end = pChunk->incIndex(midID);
        newNode->m_end = newNodeID;
        if (pChunk == m_tail)
        {
            m_tail = newNode;
        }
    }
    return newNode;
}

DwDequeList::Iterator DwDequeList::prepend()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_head);
        DW_ASSERT(NULL != m_lastAccess);
        if (m_head->full())
        {
            //为提高append性能，头结点在满时不作分裂处理，而是直接在之前创建新结点
            Chunk* newNode = allocate();
            newNode->m_next = m_head;
            m_head->m_prev = newNode;
            m_head = newNode;
        }
    }
    else
    {
        m_tail = m_head = allocate();
        m_lastAccess = static_cast<ChunkCache*>(dwMalloc(sizeof(ChunkCache)));
    }
    m_head->m_begin = m_head->decIndex(m_head->m_begin);
    ++m_count;
    m_lastAccess->update(m_head, 0, m_head->count() - 1);
    return Iterator(m_head, m_head->m_begin, 0);
}

DwDequeList::Iterator DwDequeList::append()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_tail);
        DW_ASSERT(NULL != m_lastAccess);
        if (m_tail->full())
        {
            //为提高prepend性能，尾结点在满时不作分裂处理，而是直接在之后创建新结点
            Chunk* newNode = allocate();
            newNode->m_prev = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
    }
    else
    {
        m_tail = m_head = allocate();
        m_lastAccess = static_cast<ChunkCache*>(dwMalloc(sizeof(ChunkCache)));
    }
    int listTailID = m_tail->m_end;
    m_tail->m_end = m_tail->incIndex(m_tail->m_end);
    ++m_count;
    m_lastAccess->update(m_tail, m_count - m_tail->count(), m_count - 1);
    return Iterator(m_tail, listTailID, m_count - 1);
}

DwDequeList::Iterator DwDequeList::insert(const DwDequeList::Iterator& it)
{
    if (it.equal(begin()))
    {
        return prepend();
    }
    if (it.equal(end()))
    {
        return append();
    }
    if (!validIterator(it))			//无效迭代器
    {
        return Iterator();
    }
    DW_ASSERT(NULL != m_lastAccess);
    Chunk* pChunk = it.m_pChunk;
    int offsetID = it.m_slotID;
    DW_ASSERT(offsetID != pChunk->m_end);
    int offset = pChunk->offset(pChunk->m_begin, offsetID);
    if (!(pChunk->full()))
    {
        int moveCount = 0;
        if (offset < pChunk->count() - offset)				//offset在前一半，从插入位置开始向前移动
        {
            pChunk->m_begin = pChunk->decIndex(pChunk->m_begin);
            int before = pChunk->m_begin;
            int after = pChunk->incIndex(before);
            moveCount = offset;
            for (int i = 0; i < moveCount; ++i)
            {
                pChunk->m_buf[before] = pChunk->m_buf[after];
                before = after;
                after = pChunk->incIndex(after);
            }
        }
        else												//offset在后一半，从插入位置开始向后移动
        {
            int after = pChunk->m_end;
            int before = pChunk->decIndex(after);
            moveCount = pChunk->count() - offset;
            for (int i = 0; i < moveCount; ++i)
            {
                pChunk->m_buf[after] = pChunk->m_buf[before];
                after = before;
                before = pChunk->decIndex(before);
            }
            pChunk->m_end = pChunk->incIndex(pChunk->m_end);
        }
    }
    else
    {
        //若在当前已满的结点开头插入，而此结点的前一结点又不满，则可在前一结点尾部插入
        if (0 == offset && NULL != pChunk->m_prev && !(pChunk->m_prev->full()))
        {
            DW_ASSERT(pChunk != m_head);
            pChunk = pChunk->m_prev;
            offset = pChunk->count();
            pChunk->m_end = pChunk->incIndex(pChunk->m_end);
        }
        else
        {
            pChunk = halveNode(pChunk, offset, offsetID);	//将原结点中的一半元素移动到新结点，以平衡性能
        }
    }
    ++m_count;
    m_lastAccess->update(pChunk, it.m_id - offset, it.m_id - offset + pChunk->count() - 1);
    return Iterator(pChunk, pChunk->incIndex(pChunk->m_begin, offset), it.m_id);
}

void DwDequeList::removeFirst()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_lastAccess);
        DW_ASSERT(NULL != m_head);
        --m_count;
        m_head->m_begin = m_head->incIndex(m_head->m_begin);
        if (m_head->empty())
        {
            Chunk* abandonedNode = m_head;
            m_head = m_head->m_next;
            deallocate(abandonedNode);
            if (NULL != m_head)
            {
                m_head->m_prev = NULL;
            }
            else
            {
                DW_ASSERT(0 == m_count && 0 == m_nodeCount);
                m_tail = NULL;
                dwFree(static_cast<void*>(m_lastAccess));
                m_lastAccess = NULL;
                return;
            }
        }
        m_lastAccess->update(m_head, 0, m_head->count() - 1);
    }
}

void DwDequeList::removeLast()
{
    if (0 != m_count)
    {
        DW_ASSERT(NULL != m_lastAccess);
        DW_ASSERT(NULL != m_tail);
        --m_count;
        m_tail->m_end = m_tail->decIndex(m_tail->m_end);
        if (m_tail->empty())
        {
            Chunk* abandonedNode = m_tail;
            m_tail = m_tail->m_prev;
            deallocate(abandonedNode);
            if (NULL != m_tail)
            {
                m_tail->m_next = NULL;
            }
            else
            {
                DW_ASSERT(0 == m_count && 0 == m_nodeCount);
                m_head = NULL;
                dwFree(static_cast<void*>(m_lastAccess));
                m_lastAccess = NULL;
                return;
            }
        }
        m_lastAccess->update(m_head, 0, m_head->count() - 1);
    }
}

DwDequeList::Iterator DwDequeList::remove(DwDequeList::Iterator it)
{
    DW_ASSERT(!isEmpty());
    DW_ASSERT(validIterator(it) && !it.equal(end()));
    if (it.equal(begin()))
    {
        //注：当链表中唯一的元素在end()-1的位置上时，已被it.equal(begin())处理
        removeFirst();
        return first();
    }
    //注：由于已做it.equal(begin())处理，因而以下处理后不会使DwDequeList为空
    Chunk* pChunk = it.m_pChunk;
    int offsetID = it.m_slotID;
    DW_ASSERT(offsetID != pChunk->m_end);
    int offset = pChunk->offset(pChunk->m_begin, offsetID);
    int moveCount = 0;
    if (offset < pChunk->count() - offset)					//offset在链表前一半
    {
        int after = offsetID;
        int before = pChunk->decIndex(after);
        moveCount = offset;
        for (int i = 0; i < moveCount; ++i)
        {
            pChunk->m_buf[after] = pChunk->m_buf[before];
            after = before;
            before = pChunk->decIndex(before);
        }
        pChunk->m_begin = pChunk->incIndex(pChunk->m_begin);
        it.m_slotID = it.m_pChunk->incIndex(it.m_slotID);	//删除的位置被前一个元素覆盖，需要修改迭代器
    }
    else													//offset在链表后一半
    {
        int before = offsetID;
        int after = pChunk->incIndex(before);
        moveCount = pChunk->count() - 1 - offset;			//注意：减1
        for (int i = 0; i < moveCount; ++i)
        {
            pChunk->m_buf[before] = pChunk->m_buf[after];
            before = after;
            after = pChunk->incIndex(after);
        }
        pChunk->m_end = pChunk->decIndex(pChunk->m_end);	//删除的位置被后一个元素覆盖，不需要修改迭代器
    }
    if (pChunk->empty())
    {
        Chunk* abandonedNode = pChunk;
        //即DW_ASSERT(m_head != pChunk)，已作removeFirst处理，头结点时不可能进入此判断（删除后一定还有上一个元素）
        //但可能为最后一个结点，此时删除前的此结点只有一个元素
        DW_ASSERT(NULL != abandonedNode->m_prev);
        abandonedNode->m_prev->m_next = abandonedNode->m_next;
        if(NULL != abandonedNode->m_next)					//end()的特殊性，不对称处理
        {
            abandonedNode->m_next->m_prev = abandonedNode->m_prev;
            it.m_pChunk = abandonedNode->m_next;
            it.m_slotID = it.m_pChunk->m_begin;
        }
        else
        {
            DW_ASSERT(m_tail == abandonedNode);
            m_tail = abandonedNode->m_prev;
            it = end();
        }
        deallocate(abandonedNode);
    }
    --m_count;
    m_lastAccess->update(it.m_pChunk, it.m_id - offset, it.m_id - offset + it.m_pChunk->count() - 1);
    return it;
}

void DwDequeList::clear()
{
    if (!isEmpty())
    {
        Chunk* nextNode = m_head->m_next;
        while (NULL != m_head)
        {
            deallocate(m_head);
            m_head = nextNode;
            if(NULL != nextNode)
            {
                nextNode = nextNode->m_next;
            }
        }
        m_tail = NULL;
        m_count = 0;
        dwFree(static_cast<void*>(m_lastAccess));
        m_lastAccess = NULL;
    }
}

DwDequeList::Iterator DwDequeList::createIterator(int id) const
{
    if (id <= 0)
    {
        if (0 != m_count)
        {
            m_lastAccess->update(m_head, 0, m_head->count() - 1);
        }
        return begin();
    }
    if (id >= m_count)
    {
        if (0 != m_count)
        {
            m_lastAccess->update(m_tail, m_count - m_tail->count(), m_count - 1);
        }
        return end();
    }
    DW_ASSERT(NULL != m_head);
    if (m_lastAccess->isHit(id))							//命中
    {
        return Iterator(m_lastAccess->m_pChunk
                        , m_lastAccess->m_pChunk->incIndex(m_lastAccess->m_pChunk->m_begin, id - m_lastAccess->m_minID)
                        , id);
    }
    int offset = id;
    Chunk* curNode = NULL;
    if (id < m_lastAccess->m_minID)
    {
        if (id < m_lastAccess->m_minID - id)				//在起始结点到缓存结点的一半之前
        {
            DW_ASSERT(m_lastAccess->m_minID > id);
            curNode = searchAfter(m_head, offset);
        }
        else
        {
            DW_ASSERT(NULL != m_lastAccess->m_pChunk && NULL != m_lastAccess->m_pChunk->m_prev);
            curNode = searchBefore(m_lastAccess->m_pChunk->m_prev, m_lastAccess->m_minID, offset);
        }
    }
    else
    {
        DW_ASSERT(id > m_lastAccess->m_maxID);
        if (id - m_lastAccess->m_maxID - 1 < m_count - id)	//在缓存结点到结束结点到的一半之前
        {
            DW_ASSERT(NULL != m_lastAccess->m_pChunk && NULL != m_lastAccess->m_pChunk->m_next);
            offset = offset - m_lastAccess->m_maxID -1;
            curNode = searchAfter(m_lastAccess->m_pChunk->m_next, offset);
        }
        else
        {
            DW_ASSERT(NULL != m_tail);
            curNode = searchBefore(m_tail, m_count, offset);
        }
    }
    m_lastAccess->update(curNode, id - offset, id - offset + curNode->count() - 1);
    return Iterator(curNode, curNode->incIndex(curNode->m_begin, offset), id);
}

DwDequeList::Chunk* DwDequeList::searchBefore(Chunk* pChunk, int beforeCount, int& offset) const
{
    int curEndID = beforeCount - pChunk->count();			//m_minID相当于前面的元素总数
    while (offset < curEndID)
    {
        DW_ASSERT(NULL != pChunk);
        pChunk = pChunk->m_prev;
        curEndID -= pChunk->count();
    }
    offset -= curEndID;
    return pChunk;
}

DwDequeList::Chunk* DwDequeList::searchAfter(Chunk* pChunk, int& offset) const
{
    int curNodeCount = pChunk->count();
    while (offset >= curNodeCount)
    {
        offset -= curNodeCount;
        DW_ASSERT(NULL != pChunk);
        pChunk = pChunk->m_next;
        curNodeCount = pChunk->count();
    }
    return pChunk;
}