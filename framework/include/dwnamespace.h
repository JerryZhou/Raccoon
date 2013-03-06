#pragma once

/************************************************************************
	Global enums
************************************************************************/

namespace Dw
{
enum AspectRatioMode
{
	IgnoreAspectRatio,
	KeepAspectRatio,
	KeepAspectRatioByExpanding
};

enum GraphicsSceneNotify
{
	SceneNotifyMin = 0,
	SceneNotifyAddItem = SceneNotifyMin,
	SceneNotifyAfterAdd,
	SceneNotifyRemoveItem,
	SceneNotifyAfterRemove,
	SceneNotifyRegTopLevel,
	SceneNotifyUnRegTopLevel,
	SceneNotifyDrawItem,
	SceneNotifyAfterDrawitem,
    SceneNotifyAfterDrawChild,
	SceneNotifyHoverChange,
	SceneNotifyFocusChange,
	SceneNotifyMouseGraberChange,
	SceneNotifyKeyboardGraberChange,
	SceneNotifyMax,
};

enum SortOrder 
{
	AscendingOrder,
	DescendingOrder
};

enum GraphicsItemChange {
	ItemPositionChange,
	ItemVisibleChange,
	ItemEnabledChange,
	ItemSelectedChange,
	ItemParentChange,
	ItemChildAddedChange,
	ItemChildRemovedChange,
	ItemPositionHasChanged,
	ItemSceneChange,
	ItemToolTipChange,
	ItemToolTipHasChanged,
	ItemFlagsChange,
	ItemZValueChange,
	ItemScenePositionHasChanged,
	ItemGemoryChange,
};

// NB! THE FLAG IS A uint32_t
enum GraphicsItemFlag 
{
	ItemIsMovable = 0x1,
	ItemIsSelectable = 0x2,
	ItemIsFocusable = 0x4,
	ItemClipsToShape = 0x8,
	ItemClipsChildrenToShape = 0x10,
	//ItemIgnoresTransformations = 0x20,
	//ItemIgnoresParentOpacity = 0x40,
	//ItemDoesntPropagateOpacityToChildren = 0x80,
	//ItemStacksBehindParent = 0x100,
	//ItemUsesExtendedStyleOption = 0x200,
	ItemHasNoContents = 0x400,
	//ItemSendsGeometryChanges = 0x800,
	ItemAcceptsInputMethod = 0x1000,
	//ItemNegativeZStacksBehindParent = 0x2000,
	ItemIsPanel = 0x4000,
	ItemIsFocusScope = 0x8000, 
	ItemIsHoverable = 0x10000,
	// internal
	/*ItemSendsScenePositionChanges = 0x10000,*/
	ItemStopsClickFocusPropagation = 0x20000,
	//ItemOnlyAcceptTranslate = 0x40000,
	ItemSelfKeyFocus = 0x80000,
	ItemStopsMousePropagation = 0x100000,
    ItemClipsChildren = 0x200000,
    ItemAccessible = 0x400000,
    ItemRequireRawMessageWhenHover = 0x800000,
    ItemRequireRawMessageWhenFocus = 0x1000000,
};

enum GraphicsItemType
{
	ItemType = 1,
    ItemPixmap,
	ItemSimpleText,
	ItemRect,
	ItemPositioner,
	ItemUserType = 65536
};

enum GraphicsItemDirty
{
	ChildrenDirty,
	ChildrenBoundingDirty,
	AllChildrenDirty,
};

enum Axis 
{
	XAxis,
	YAxis,
	ZAxis
};

enum Initialization 
{
	Uninitialized
};

// Shape = 0x1, BoundingRect = 0x2
enum ItemSelectionMode 
{
	ContainsItemShape = 0x0,
	IntersectsItemShape = 0x1,
	ContainsItemBoundingRect = 0x2,
	IntersectsItemBoundingRect = 0x3
};

enum ClipOperation 
{
	NoClip,
	ReplaceClip,
	IntersectClip,
	UniteClip
};

// NB! DO NOT CHANGE THE ORDER
enum EventDirection
{
	DeliverUp = 0,
	DeliverDown = 1,
};

enum SceneIndicator
{
	SceneIndicatorMin = 0,
	LastHoverItem = SceneIndicatorMin,
	FocusItem,
	LastFocusItem,
	MouseGraber,
	KeyBoardGraber,
	TabFirstFocus,

	// just used to mark a item, there are 8 slots
	UserIndicator0,
	UserIndicator1,
	UserIndicator2,
	UserIndicator3,
    UserIndicator4,
    UserIndicator5,
    UserIndicator6,
    UserIndicator7,
	SceneIndicatorMax,
};

enum ViewIndicator
{
    WindowItem,
    ViewIndicatorMax,
};

// NB! DO NOT CHANGE THE ORDER
enum AnchorLine 
{
	Invalid = 0x0,
	Left = 0x01,
	Right = 0x02,
	Top = 0x04,
	Bottom = 0x08,
	HCenter = 0x10,
	VCenter = 0x20,
	Baseline = 0x40,
	Horizontal_Mask = Left | Right | HCenter,
	Vertical_Mask = Top | Bottom | VCenter | Baseline,
};

enum Anchor 
{
	LeftAnchor = 0x01,
	RightAnchor = 0x02,
	TopAnchor = 0x04,
	BottomAnchor = 0x08,
	HCenterAnchor = 0x10,
	VCenterAnchor = 0x20,
	BaselineAnchor = 0x40,
	HorizontalAnchor_Mask = LeftAnchor | RightAnchor | HCenterAnchor,
	VerticalAnchor_Mask = TopAnchor | BottomAnchor | VCenterAnchor | BaselineAnchor,
	CenterAnchor_Mask = HCenterAnchor | VCenterAnchor,
	InvalidAnchor,
};

enum ChangeType 
{
	Geometry = 0x01,
	SiblingOrder = 0x02,
	Visibility = 0x04,
	Opacity = 0x08,
	Destroyed = 0x10,
	Position = 0x20,
};

enum OwnerShip
{
	ListenerOwner,
	ItemOwner,
};

enum KeyboardModifier 
{
    NoModifier           = 0x00000000,
    ShiftModifier        = 0x02000000,
    ControlModifier      = 0x04000000,
    AltModifier          = 0x08000000,
    MetaModifier         = 0x10000000,
    KeypadModifier       = 0x20000000,
    GroupSwitchModifier  = 0x40000000,
    // Do not extend the mask to include 0x01000000
    KeyboardModifierMask = 0xfe000000
};

//shorter names for shortcuts
enum Modifier 
{
    META          = Dw::MetaModifier,
    SHIFT         = Dw::ShiftModifier,
    CTRL          = Dw::ControlModifier,
    ALT           = Dw::AltModifier,
    MODIFIER_MASK = KeyboardModifierMask,
    UNICODE_ACCEL = 0x00000000
};

enum MouseButton 
{
    NoButton         = 0x00000000,
    LeftButton       = 0x00000001,
    RightButton      = 0x00000002,
    MiddleButton     = 0x00000004,
    XButton1         = 0x00000008,
    XButton2         = 0x00000010,
    MouseButtonMask  = 0x000000ff
};

// NB! DO NOT MODIFI, MAPPING TO SYSTEM
enum MouseVirtualKeyState
{
    MKNOBUTTON        = 0x0000,
    MKLBUTTON         = 0x0001,
    MKRBUTTON         = 0x0002,
    MKSHIFT           = 0x0004,
    MKCONTROL         = 0x0008,
    MKMBUTTON         = 0x0010,
    MKXBUTTON1        = 0x0020,
    MKXBUTTON2        = 0x0040,
};

enum FocusReason 
{
	MouseFocusReason,		// support
	TabFocusReason,			// no support now
	BacktabFocusReason,		// no support now
	ActiveWindowFocusReason,// no support now
	PopupFocusReason,		// no support now
	ShortcutFocusReason,	// no support now
	MenuBarFocusReason,		// no support now
	OtherFocusReason,		// no support now
	NoFocusReason			// no support now
};

enum FocusPolicy 
{
	NoFocus = 0,
	TabFocus = 0x1,
	ClickFocus = 0x2,
	StrongFocus = TabFocus | ClickFocus | 0x8,
	WheelFocus = StrongFocus | 0x4
};

enum AlignmentFlag 
{
	AlignLeft = 0x0001,
	AlignLeading = AlignLeft,
	AlignRight = 0x0002,
	AlignTrailing = AlignRight,
	AlignHCenter = 0x0004,
	AlignJustify = 0x0008,
	AlignAbsolute = 0x0010,
	AlignHorizontal_Mask = AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,

	AlignTop = 0x0020,
	AlignBottom = 0x0040,
	AlignVCenter = 0x0080,
	AlignVertical_Mask = AlignTop | AlignBottom | AlignVCenter,

	AlignCenter = AlignVCenter | AlignHCenter
};

enum Curve
{
	Linear,
	InQuad, OutQuad, InOutQuad, OutInQuad,
	InCubic, OutCubic, InOutCubic, OutInCubic,
	InQuart, OutQuart, InOutQuart, OutInQuart,
	InQuint, OutQuint, InOutQuint, OutInQuint,
	InSine, OutSine, InOutSine, OutInSine,
	InExpo, OutExpo, InOutExpo, OutInExpo,
	InCirc, OutCirc, InOutCirc, OutInCirc,
	InElastic, OutElastic, InOutElastic, OutInElastic,
	InBack, OutBack, InOutBack, OutInBack,
	InBounce, OutBounce, InOutBounce, OutInBounce,
	InCurve, OutCurve, SineCurve, CosineCurve,
	Custom, NCurveTypes
};

enum Direction 
{
	Forward,
	Backward
};

enum State 
{
	Stopped,
	Paused,
	Running
};

enum DeletionPolicy 
{
	KeepWhenStopped = 0,
	DeleteWhenStopped
};

enum ClockType 
{
	SystemTime,
	MonotonicClock,
	TickCounter,
	MachAbsoluteTime
};

enum FontStyle
{
	FontStyleRegular = 0,
	FontStyleBold = 1,
	FontStyleItalic = 2,
	FontStyleBoldItalic = 3,
	FontStyleUnderline = 4,
	FontStyleStrikeout = 8
};

enum SceneLayer 
{
	ItemLayer = 0x1,
	BackgroundLayer = 0x2,
	ForegroundLayer = 0x4,
	AllLayers = 0xffff
};

enum ViewportUpdateMode 
{
	FullViewportUpdate,	// Support
	SmartViewportUpdate,	// Support
	NoViewportUpdate,	// Support
	BoundingRectViewportUpdate // Support
};

enum WidgetAttribute 
{
	//WA_Disabled = 0,
	//WA_UnderMouse = 1,
	//WA_MouseTracking = 2,
	//WA_ContentsPropagated = 3, // ## deprecated
	//WA_OpaquePaintEvent = 4,
	//WA_NoBackground = WA_OpaquePaintEvent, // ## deprecated
	//WA_StaticContents = 5,
	//WA_LaidOut = 7,
	//WA_PaintOnScreen = 8,
	//WA_NoSystemBackground = 9,
	//WA_UpdatesDisabled = 10,
	//WA_Mapped = 11,
	//WA_MacNoClickThrough = 12, // Mac only
	//WA_PaintOutsidePaintEvent = 13,
	//WA_InputMethodEnabled = 14,
	//WA_WState_Visible = 15,
	//WA_WState_Hidden = 16,

	//WA_ForceDisabled = 32,
	//WA_KeyCompression = 33,
	//WA_PendingMoveEvent = 34,
	//WA_PendingResizeEvent = 35,
	//WA_SetPalette = 36,
	//WA_SetFont = 37,
	//WA_SetCursor = 38,
	//WA_NoChildEventsFromChildren = 39,
	//WA_WindowModified = 41,
	//WA_Resized = 42,
	//WA_Moved = 43,
	//WA_PendingUpdate = 44,
	//WA_InvalidSize = 45,
	//WA_MacBrushedMetal = 46, // Mac only
	//WA_MacMetalStyle = WA_MacBrushedMetal, // obsolete
	//WA_CustomWhatsThis = 47,
	//WA_LayoutOnEntireRect = 48,
	//WA_OutsideWSRange = 49,
	//WA_GrabbedShortcut = 50,
	//WA_TransparentForMouseEvents = 51,
	//WA_PaintUnclipped = 52,
	//WA_SetWindowIcon = 53,
	//WA_NoMouseReplay = 54,
	WA_DeleteOnClose = 55,
	//WA_RightToLeft = 56,
	//WA_SetLayoutDirection = 57,
	//WA_NoChildEventsForParent = 58,
	//WA_ForceUpdatesDisabled = 59,

	//WA_WState_Created = 60,
	//WA_WState_CompressKeys = 61,
	//WA_WState_InPaintEvent = 62,
	//WA_WState_Reparented = 63,
	//WA_WState_ConfigPending = 64,
	//WA_WState_Polished = 66,
	//WA_WState_DND = 67, // ## deprecated
	//WA_WState_OwnSizePolicy = 68,
	//WA_WState_ExplicitShowHide = 69,

	//WA_ShowModal = 70, // ## deprecated
	//WA_MouseNoMask = 71,
	//WA_GroupLeader = 72, // ## deprecated
	//WA_NoMousePropagation = 73, // ## for now, might go away.
	//WA_Hover = 74,
	//WA_InputMethodTransparent = 75, // Don't reset IM when user clicks on this (for virtual keyboards on embedded)
	//WA_QuitOnClose = 76,

	//WA_KeyboardFocusChange = 77,

	//WA_AcceptDrops = 78,
	//WA_DropSiteRegistered = 79, // internal
	//WA_ForceAcceptDrops = WA_DropSiteRegistered, // ## deprecated

	//WA_WindowPropagation = 80,

	//WA_NoX11EventCompression = 81,
	//WA_TintedBackground = 82,
	//WA_X11OpenGLOverlay = 83,
	//WA_AlwaysShowToolTips = 84,
	//WA_MacOpaqueSizeGrip = 85,
	//WA_SetStyle = 86,

	//WA_SetLocale = 87,
	//WA_MacShowFocusRect = 88,

	//WA_MacNormalSize = 89,  // Mac only
	//WA_MacSmallSize = 90,   // Mac only
	//WA_MacMiniSize = 91,    // Mac only

	//WA_LayoutUsesWidgetRect = 92,
	//WA_StyledBackground = 93, // internal
	//WA_MSWindowsUseDirect3D = 94, // Win only
	//WA_CanHostQMdiSubWindowTitleBar = 95, // Internal

	//WA_MacAlwaysShowToolWindow = 96, // Mac only

	//WA_StyleSheet = 97, // internal

	//WA_ShowWithoutActivating = 98,

	//WA_X11BypassTransientForHint = 99,

	//WA_NativeWindow = 100,
	//WA_DontCreateNativeAncestors = 101,

	//WA_MacVariableSize = 102,    // Mac only

	WA_DontShowOnScreen = 103,

	//// window types from http://standards.freedesktop.org/wm-spec/
	//WA_X11NetWmWindowTypeDesktop = 104,
	//WA_X11NetWmWindowTypeDock = 105,
	//WA_X11NetWmWindowTypeToolBar = 106,
	//WA_X11NetWmWindowTypeMenu = 107,
	//WA_X11NetWmWindowTypeUtility = 108,
	//WA_X11NetWmWindowTypeSplash = 109,
	//WA_X11NetWmWindowTypeDialog = 110,
	//WA_X11NetWmWindowTypeDropDownMenu = 111,
	//WA_X11NetWmWindowTypePopupMenu = 112,
	//WA_X11NetWmWindowTypeToolTip = 113,
	//WA_X11NetWmWindowTypeNotification = 114,
	//WA_X11NetWmWindowTypeCombo = 115,
	//WA_X11NetWmWindowTypeDND = 116,

	//WA_MacFrameworkScaled  = 117,

	//WA_SetWindowModality = 118,
	//WA_WState_WindowOpacitySet = 119, // internal
	//WA_TranslucentBackground = 120,

	//WA_AcceptTouchEvents = 121,
	//WA_WState_AcceptedTouchBeginEvent = 122,
	//WA_TouchPadAcceptSingleTouchEvents = 123,

	//WA_MergeSoftkeys =  124,
	//WA_MergeSoftkeysRecursively =  125,

	//WA_LockPortraitOrientation = 128,
	//WA_LockLandscapeOrientation = 129,
	//WA_AutoOrientation = 130,

	//WA_X11DoNotAcceptFocus = 132,

    WA_ResizeAble = 133,
	// Add new attributes before this line
    WA_InnerToolTip = 134,
	WA_AttributeCount
};

enum CompositingMode	// THE SAME SEQ TO GDI
{
	CompositingModeSourceOver,    // 0
	CompositingModeSourceCopy     // 1
};

enum TextRenderingHint
{
    TextRenderingHintSystemDefault = 0,            // Glyph with system default rendering hint
    TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
    TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
    TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
    TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
    TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
};

enum QualityMode
{
    QualityModeInvalid   = -1,
    QualityModeDefault   = 0,
    QualityModeLow       = 1, // Best performance
    QualityModeHigh      = 2  // Best rendering quality
};

enum InterpolationMode
{
    InterpolationModeInvalid          = QualityModeInvalid,
    InterpolationModeDefault          = QualityModeDefault,
    InterpolationModeLowQuality       = QualityModeLow,
    InterpolationModeHighQuality      = QualityModeHigh,
    InterpolationModeBilinear,
    InterpolationModeBicubic,
    InterpolationModeNearestNeighbor,
    InterpolationModeHighQualityBilinear,
    InterpolationModeHighQualityBicubic
};

//--------------------------------------------------------------------------
// Alpha Compositing quality constants
//--------------------------------------------------------------------------

enum CompositingQuality
{
    CompositingQualityInvalid          = QualityModeInvalid,
    CompositingQualityDefault          = QualityModeDefault,
    CompositingQualityHighSpeed        = QualityModeLow,
    CompositingQualityHighQuality      = QualityModeHigh,
    CompositingQualityGammaCorrected,
    CompositingQualityAssumeLinear
};
enum CursorShape 
{
    ArrowCursor,
    UpArrowCursor,
    CrossCursor,
    WaitCursor,
    IBeamCursor,
    SizeVerCursor,
    SizeHorCursor,
    SizeBDiagCursor,
    SizeFDiagCursor,
    SizeAllCursor,
    BlankCursor,
    SplitVCursor,
    SplitHCursor,
    PointingHandCursor,
    ForbiddenCursor,
    WhatsThisCursor,
    BusyCursor,
    OpenHandCursor,
    ClosedHandCursor,
    DragCopyCursor,
    DragMoveCursor,
    DragLinkCursor,
    LastCursor = DragLinkCursor,
    BitmapCursor = 24,
    CustomCursor = 25
};

enum WinStyle
{
    AppWindow,
    ToolWindow,
    PopupWindow,
    WinStyleMax,
};

enum ItemHook
{
    HookCreate,
    HookDestroy,
    HookUpdate,
};

enum ShowCommand
{
    SW_Hide = 0,
    SW_ShowNormal = 1,
    SW_Normal = 1,
    SW_ShowMinimized = 2,
    SW_ShowMaximized = 3,
    SW_Maximize = 3,
    SW_ShowNoActivate = 4,
    SW_Show = 5,
    SW_Minimize = 6,
    SW_ShowMinNoActive = 7,
    SW_ShowNA = 8,
    SW_Restore = 9,
    SW_ShowDefault = 10,
    SW_ForceMinimize = 11,
    SW_Max = 11,
};

enum CheckedState
{
    UnChecked,
    Checked,
};

enum GlobalColor 
{
    black,
    white,
    darkGray,
    gray,
    lightGray,
    red,
    green,
    blue,
    cyan,
    magenta,
    yellow,
    darkRed,
    darkGreen,
    darkBlue,
    darkCyan,
    darkMagenta,
    darkYellow,
    transparent
};
}