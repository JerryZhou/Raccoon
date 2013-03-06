#pragma once
#include "dwnamespace.h"

//------------------------------------------------------------------------------
namespace DwWidget
{
enum UserItemType
{
    ItemTypeWidget = Dw::ItemUserType,
    FrameItem,
};

enum FrameState
{
    SNormal = 0,
    SHover,
    SPressDown,
    SFocus,
    SDisable,
    SStateMax,
};

enum ProgressPhase
{
    PhaseLeft = 0,
    PhaseMiddle,
    PhaseRight,
    PhaseMax,
};

enum Flow
{
    LeftToRight,
    TopToBottom
};

enum DragPolicy
{
    NoDrag,
    DragWindow,
    AsParent,
};

enum EditTextChange
{
    Ops,
    AddChar,
    DelChar,
};

enum MessageBoxButton
{
    MBB_OK,
    MBB_CANCLE,
    MBB_CLOSED,
};

enum MessageBoxMode
{
    MBM_OK ,
    MBM_OKCANCEL ,
};
}// end of namespace DwWidget
//------------------------------------------------------------------------------