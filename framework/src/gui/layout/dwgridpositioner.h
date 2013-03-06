#pragma once
#include "dwbasepositioner.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGridPositioner : public DwBasePositioner
{
    DW_DECLARE_CLASS(DwGridPositioner);
public:
    DwGridPositioner(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwGridPositioner();

    inline int rows() const;
    void setRows(const int rows);

    inline int columns() const;
    void setColumns(const int columns);

    enum Flow { LeftToRight, TopToBottom };
    inline Flow flow() const;
    void setFlow(Flow);

protected:
    virtual void doPositioning(DwSizeF *contentSize);
    virtual void reportConflictingAnchors();

    int m_rows;
    int m_columns;
    Flow m_flow;
private:
};// end of DwGridPositioner
DW_REGISTER_CLASS(DwGridPositioner);

//------------------------------------------------------------------------------
/**
*/
inline int DwGridPositioner::rows() const
{
    return m_rows;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGridPositioner::columns() const
{
    return m_columns;
}

//------------------------------------------------------------------------------
/**
*/
inline DwGridPositioner::Flow DwGridPositioner::flow() const
{
    return m_flow;
}