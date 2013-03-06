#pragma once

#include "dwcore/dwvector.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwvariant.h"
#include "dwcore/dwlinkedlist.h"
#include "dwgui/dwrttiobject.h"

#include <initguid.h>
#include <oleacc.h>
#include <comdef.h>

//------------------------------------------------------------------------------
class DwAccessibleInterface;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAccessible : public DwRttiObject
{
    DW_DECLARE_CLASS(DwAccessible);
public:
    enum Event {
        SoundPlayed          = 0x0001,
        Alert                = 0x0002,
        ForegroundChanged    = 0x0003,
        MenuStart            = 0x0004,
        MenuEnd              = 0x0005,
        PopupMenuStart       = 0x0006,
        PopupMenuEnd         = 0x0007,
        ContextHelpStart     = 0x000C,
        ContextHelpEnd       = 0x000D,
        DragDropStart        = 0x000E,
        DragDropEnd          = 0x000F,
        DialogStart          = 0x0010,
        DialogEnd            = 0x0011,
        ScrollingStart       = 0x0012,
        ScrollingEnd         = 0x0013,

        MenuCommand          = 0x0018,

        ObjectCreated        = 0x8000,
        ObjectDestroyed      = 0x8001,
        ObjectShow           = 0x8002,
        ObjectHide           = 0x8003,
        ObjectReorder        = 0x8004,
        Focus                = 0x8005,
        Selection            = 0x8006,
        SelectionAdd         = 0x8007,
        SelectionRemove      = 0x8008,
        SelectionWithin      = 0x8009,
        StateChanged         = 0x800A,
        LocationChanged      = 0x800B,
        NameChanged          = 0x800C,
        DescriptionChanged   = 0x800D,
        ValueChanged         = 0x800E,
        ParentChanged        = 0x800F,
        HelpChanged          = 0x80A0,
        DefaultActionChanged = 0x80B0,
        AcceleratorChanged   = 0x80C0
    };

    enum StateFlag {
        Normal          = 0x00000000,
        Unavailable     = 0x00000001,
        Selected        = 0x00000002,
        Focused         = 0x00000004,
        Pressed         = 0x00000008,
        Checked         = 0x00000010,
        Mixed           = 0x00000020,
        ReadOnly        = 0x00000040,
        HotTracked      = 0x00000080,
        DefaultButton   = 0x00000100,
        Expanded        = 0x00000200,
        Collapsed       = 0x00000400,
        Busy            = 0x00000800,
        // Floating        = 0x00001000,
        Marqueed        = 0x00002000,
        Animated        = 0x00004000,
        Invisible       = 0x00008000,
        Offscreen       = 0x00010000,
        Sizeable        = 0x00020000,
        Movable         = 0x00040000,

        SelfVoicing     = 0x00080000,
        Focusable       = 0x00100000,
        Selectable      = 0x00200000,
        Linked          = 0x00400000,
        Traversed       = 0x00800000,
        MultiSelectable = 0x01000000,
        ExtSelectable   = 0x02000000,

        Protected       = 0x20000000,
        HasPopup        = 0x40000000,
        Modal           = 0x80000000,

        HasInvokeExtension = 0x10000000 // internal
    };
    typedef uint32_t State;

        enum Role {
            NoRole         = 0x00000000,
            TitleBar       = 0x00000001,
            MenuBar        = 0x00000002,
            ScrollBar      = 0x00000003,
            Grip           = 0x00000004,
            Sound          = 0x00000005,
            Cursor         = 0x00000006,
            Caret          = 0x00000007,
            AlertMessage   = 0x00000008,
            Window         = 0x00000009,
            Client         = 0x0000000A,
            PopupMenu      = 0x0000000B,
            MenuItem       = 0x0000000C,
            ToolTip        = 0x0000000D,
            Application    = 0x0000000E,
            Document       = 0x0000000F,
            Pane           = 0x00000010,
            Chart          = 0x00000011,
            Dialog         = 0x00000012,
            Border         = 0x00000013,
            Grouping       = 0x00000014,
            Separator      = 0x00000015,
            ToolBar        = 0x00000016,
            StatusBar      = 0x00000017,
            Table          = 0x00000018,
            ColumnHeader   = 0x00000019,
            RowHeader      = 0x0000001A,
            Column         = 0x0000001B,
            Row            = 0x0000001C,
            Cell           = 0x0000001D,
            Link           = 0x0000001E,
            HelpBalloon    = 0x0000001F,
            Assistant      = 0x00000020,
            List           = 0x00000021,
            ListItem       = 0x00000022,
            Tree           = 0x00000023,
            TreeItem       = 0x00000024,
            PageTab        = 0x00000025,
            PropertyPage   = 0x00000026,
            Indicator      = 0x00000027,
            Graphic        = 0x00000028,
            StaticText     = 0x00000029,
            EditableText   = 0x0000002A,  // Editable, selectable, etc.
            PushButton     = 0x0000002B,
            CheckBox       = 0x0000002C,
            RadioButton    = 0x0000002D,
            ComboBox       = 0x0000002E,
            // DropList       = 0x0000002F,
            ProgressBar    = 0x00000030,
            Dial           = 0x00000031,
            HotkeyField    = 0x00000032,
            Slider         = 0x00000033,
            SpinBox        = 0x00000034,
            Canvas         = 0x00000035,
            Animation      = 0x00000036,
            Equation       = 0x00000037,
            ButtonDropDown = 0x00000038,
            ButtonMenu     = 0x00000039,
            ButtonDropGrid = 0x0000003A,
            Whitespace     = 0x0000003B,
            PageTabList    = 0x0000003C,
            Clock          = 0x0000003D,
            Splitter       = 0x0000003E,
            // Additional roles where enum value does not map directly to MSAA:
            LayeredPane    = 0x0000003F,
            UserRole       = 0x0000ffff
    };

    enum Text {
        Name         = 0,
        Description,
        Value,
        Help,
        Accelerator,
        UserText     = 0x0000ffff
    };

    enum RelationFlag {
        Unrelated     = 0x00000000,
        Self          = 0x00000001,
        Ancestor      = 0x00000002,
        Child         = 0x00000004,
        Descendent    = 0x00000008,
        Sibling       = 0x00000010,
        HierarchyMask = 0x000000ff,

        Up            = 0x00000100,
        Down          = 0x00000200,
        Left          = 0x00000400,
        Right         = 0x00000800,
        Covers        = 0x00001000,
        Covered       = 0x00002000,
        GeometryMask  = 0x0000ff00,

        FocusChild    = 0x00010000,
        Label         = 0x00020000,
        Labelled      = 0x00040000,
        Controller    = 0x00080000,
        Controlled    = 0x00100000,
        LogicalMask   = 0x00ff0000
    };
    typedef uint32_t Relation;

    enum Action {
        DefaultAction       = 0,
        Press               = -1,
        FirstStandardAction = Press,
        SetFocus            = -2,
        Increase            = -3,
        Decrease            = -4,
        Accept              = -5,
        Cancel	            = -6,
        Select              = -7,
        ClearSelection      = -8,
        RemoveSelection     = -9,
        ExtendSelection     = -10,
        AddToSelection      = -11,
        LastStandardAction  = AddToSelection
    };

    enum Method {
        ListSupportedMethods      = 0,
        SetCursorPosition         = 1,
        GetCursorPosition         = 2,
        ForegroundColor           = 3,
        BackgroundColor           = 4
    };

    typedef DwAccessibleInterface*(*InterfaceFactory)(DwRttiObject*);
    typedef void(*UpdateHandler)(DwRttiObject*, int who, Event reason);
    typedef void(*RootObjectHandler)(DwRttiObject*);

    static void installFactory(const DwRtti* key, InterfaceFactory);
    static void removeFactory(const DwRtti* key);
    static UpdateHandler installUpdateHandler(UpdateHandler);
    static RootObjectHandler installRootObjectHandler(RootObjectHandler);

    static DwAccessibleInterface *queryAccessibleInterface(DwRttiObject *);
    static void updateAccessibility(DwRttiObject *, int who, Event reason);
    static bool isActive();
    static void setRootObject(DwRttiObject*);

    static void initialize();
    static void cleanup();

private:
    static UpdateHandler updateHandler;
    static RootObjectHandler rootObjectHandler;
};

typedef DwVector<DwVariant> DwVariantList;
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAccessibleInterface : public DwAccessible
{
    DW_DECLARE_ABSTRACT_CLASS(DwAccessibleInterface);
public:
    virtual ~DwAccessibleInterface() {}
    // check for valid pointers
    virtual bool isValid() const = 0;
    virtual DwRttiObject *object() const = 0;

    // hierarchy
    virtual int childCount() const = 0;
    virtual int indexOfChild(const DwAccessibleInterface *) const = 0;

    // relations
    virtual Relation relationTo(int child, const DwAccessibleInterface *other,
        int otherChild) const = 0;
    virtual int childAt(int x, int y) const = 0;

    // navigation
    virtual int navigate(RelationFlag relation, int index, DwAccessibleInterface **iface) const = 0;

    // properties and state
    virtual DwString text(Text t, int child) const = 0;
    virtual void setText(Text t, int child, const DwString &text) = 0;
    virtual DwRect rect(int child) const = 0;
    virtual Role role(int child) const = 0;
    virtual State state(int child) const = 0;

    // action
    virtual int userActionCount(int child) const = 0;
    virtual DwString actionText(int action, Text t, int child) const = 0;
    virtual bool doAction(int action, int child, const DwVariantList &params = DwVariantList()) = 0;
};// end of DwAccessibleInterface