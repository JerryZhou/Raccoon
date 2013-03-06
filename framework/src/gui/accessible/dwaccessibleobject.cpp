#include "stable.h"
#include "dwaccessibleobject.h"

DW_IMPLEMENT_ABSTRACT_CLASS(DwAccessibleObject, 'ABOT', DwAccessibleInterface);

//------------------------------------------------------------------------------
/**
*/
DwAccessibleObject::DwAccessibleObject(DwRttiObject *object)
{
    m_object = object;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleObject::~DwAccessibleObject()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject *DwAccessibleObject::object() const
{
    return m_object;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAccessibleObject::isValid() const
{
    return !m_object.isNull();
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwAccessibleObject::rect(int) const
{
    return DwRect();
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleObject::setText(Text, int, const DwString &)
{
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleObject::userActionCount(int) const
{
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAccessibleObject::doAction(int, int, const DwVariantList &)
{
    return false;
}

static const char * const action_text[][5] =
{
    // Name, Description, Value, Help, Accelerator
    { "Press", "", "", "", "Space" },
    { "SetFocus", "Passes focus to this widget", "", "", "" },
    { "Increase", "", "", "", "" },
    { "Decrease", "", "", "", "" },
    { "Accept", "", "", "", "" },
    { "Cancel", "", "", "", "" },
    { "Select", "", "", "", "" },
    { "ClearSelection", "", "", "", "" },
    { "RemoveSelection", "", "", "", "" },
    { "ExtendSelection", "", "", "", "" },
    { "AddToSelection", "", "", "", "" }
};

//------------------------------------------------------------------------------
/**
*/
DwString DwAccessibleObject::actionText(int action, Text t, int child) const
{
    if (child || action > FirstStandardAction || action < LastStandardAction || t > Accelerator)
        return DwString();

    return DwString::fromAscii(action_text[-(action - FirstStandardAction)][t]);
}