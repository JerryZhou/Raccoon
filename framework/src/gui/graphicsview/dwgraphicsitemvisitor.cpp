#include "stable.h"
#include "dwgraphicsitemvisitor.h"
#include "dwgraphicsitem.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemVisitor::DwGraphicsItemVisitor()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemVisitor::~DwGraphicsItemVisitor()
{

}

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemIterator::DwGraphicsItemIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
    : m_root(NULL)
    , m_visitor(NULL)
{
    initIterator(root, visitor);
}

DwGraphicsItemIterator::~DwGraphicsItemIterator()
{

}

//------------------------------------------------------------------------------
/**
    will go through the tree, if nCount = -1, will viste all tree,
    otherwise, will viste only nCount tree item
*/
int DwGraphicsItemIterator::run(int nCount/* = -1*/)
{
    if (nCount > 0 || nCount == -1)
    {
        DwGraphicsItem* nextItem = NULL;
        int cout = nCount;
        do
        {
            nextItem = next();
            if (nextItem)
            {
                m_visitor->interest(nextItem);
            }
            else
            {
                break;
            }
            --cout;
        }
        while((cout > 0 || nCount == -1));
        return (nCount-cout);
    }
    return 0;
}

void DwGraphicsItemIterator::initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
{
    m_root = root;
    m_visitor = visitor;
}

//////////////////////////////////////////////////////////////////////////
DwGraphicsItemPreorderIterator::DwGraphicsItemPreorderIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
    : DwGraphicsItemIterator(root, visitor)
{
    m_childIterator = NULL;
    initIterator(root, visitor);
    //std::cout << "preorder constructor: " << std::hex << (int)(this) << std::oct <<std::endl;
}

DwGraphicsItemPreorderIterator::~DwGraphicsItemPreorderIterator()
{
    if (m_childIterator)
    {
        delete m_childIterator;
        m_childIterator = NULL;
    }
    //std::cout << "preorder destructor: " << std::hex << (int)(this) << std::oct <<std::endl;
}

DwGraphicsItem* DwGraphicsItemPreorderIterator::next()
{
    if (!m_root)
    {
        return NULL;
    }

    if (!m_rootVisited)
    {
        m_rootVisited = true;
        return m_root;
    }
    else
    {
        if (m_curVisited < m_root->children().size())
        {
            if (!m_childIterator)
            {
                // can be back to front
                DW_ASSERT(m_curVisited == 0);
                m_childIterator = new DwGraphicsItemPreorderIterator(m_root->children().at(m_curVisited), m_visitor);
            }

            DwGraphicsItem* nextItem = m_childIterator->next();

            if (!nextItem)
            {
                ++m_curVisited;
                if (m_curVisited < m_root->children().size())
                {
                    // can be back to front
                    m_childIterator->initIterator(m_root->children().at(m_curVisited), m_visitor);
                    return next();
                }
                else
                {
                    delete m_childIterator;
                    m_childIterator = NULL;
                    return NULL;
                }
            }
            return nextItem;

        }
        return NULL;
    }
}

void DwGraphicsItemPreorderIterator::initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
{
    DwGraphicsItemIterator::initIterator(root, visitor);
    m_curVisited = 0;
    m_rootVisited = false;
    if (m_childIterator)
    {
        delete m_childIterator;
        m_childIterator = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemPostorderIterator::DwGraphicsItemPostorderIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
    : DwGraphicsItemIterator(root, visitor)
{
    m_childIterator = NULL;
    initIterator(root, visitor);
    //std::cout << "postorder constructor: " << std::hex << (int)(this) << std::oct <<std::endl;
}

DwGraphicsItemPostorderIterator::~DwGraphicsItemPostorderIterator()
{
    if (m_childIterator)
    {
        delete m_childIterator;
        m_childIterator = NULL;
    }
    //std::cout << "postorder destructor: " << std::hex << (int)(this) << std::oct <<std::endl;
}

DwGraphicsItem* DwGraphicsItemPostorderIterator::next()
{
    if (!m_root)
    {
        return NULL;
    }

    if (m_curVisited < m_root->children().size())
    {
        if (!m_childIterator)
        {
            // can be back to front
            DW_ASSERT(m_curVisited == 0);
            m_childIterator = new DwGraphicsItemPostorderIterator(m_root->children().at(m_root->children().size() - 1 - m_curVisited), m_visitor);
        }

        DwGraphicsItem* nextItem = m_childIterator->next();
        if (!nextItem)
        {
            ++m_curVisited;
            if (m_curVisited < m_root->children().size())
            {
                // can be back to front
                m_childIterator->initIterator(m_root->children().at(m_root->children().size() - 1 - m_curVisited), m_visitor);
                return next();
            }
            else
            {
                delete m_childIterator;
                m_childIterator = NULL;
                // route to root and root not visited
                DW_ASSERT(!m_rootVisited);
                return next();
            }
        }
        return nextItem;
    }
    else
    {
        if (!m_rootVisited)
        {
            m_rootVisited = true;
            return m_root;
        }
        else
        {
            return NULL;
        }
    }
}

void DwGraphicsItemPostorderIterator::initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor)
{
    DwGraphicsItemIterator::initIterator(root, visitor);
    m_curVisited = 0;
    m_rootVisited = false;
    if (m_childIterator)
    {
        delete m_childIterator;
        m_childIterator = NULL;
    }
}
