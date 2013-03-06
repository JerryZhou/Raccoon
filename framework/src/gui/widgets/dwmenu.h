#pragma once
#include "dwgui/dwframewindow.h"

//------------------------------------------------------------------------------
class DwColumnPositioner;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMenu : public DwFrameWindow
{
    DW_DECLARE_CLASS(DwMenu);
public:
    explicit DwMenu(DwObject *parent = NULL);
    virtual ~DwMenu();

    DwGraphicsItem* addAction(const DwString& name, const DwString& txt, const DwUrl& icon, int iconIdx = 0);
    DwGraphicsItem* addLine(const DwString& name, const DwColor& c);
    DwGraphicsItem* findAction(const DwString& name) const;
    void setActionIcon(const DwString& name, const DwUrl& icon, int iconIdx = 0);
    void setActionText(const DwString& name, const DwString& txt);
    bool remvoeAction(const DwString& name);
    bool showAction(const DwString& name, bool b);

protected:
    void onContentChange(DwRttiObject* obj, DwEvent* evt);

    DwMargins m_margins;
    DwRttiObjectPtr<DwMenu> m_parent;
    DwRttiObjectPtr<DwMenu> m_subMenu;
    DwColumnPositioner* m_content;
};// end of DwMenu