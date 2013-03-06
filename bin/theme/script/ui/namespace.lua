module("Dw")

-- FocusPolicy 
--{
NoFocus = 0
TabFocus = 0x1
ClickFocus = 0x2
StrongFocus = TabFocus + ClickFocus + 0x8
WheelFocus = StrongFocus + 0x4
--};

--enum MouseButton 
--{
NoButton         = 0x00000000
LeftButton       = 0x00000001
RightButton      = 0x00000002
MiddleButton     = 0x00000004
XButton1         = 0x00000008
XButton2         = 0x00000010
MouseButtonMask  = 0x000000ff
--};

--enum GraphicsItemFlag 
--{
ItemIsMovable = 0x1
ItemIsSelectable = 0x2
ItemIsFocusable = 0x4
ItemClipsToShape = 0x8
ItemClipsChildrenToShape = 0x10
ItemHasNoContents = 0x400
ItemAcceptsInputMethod = 0x1000
ItemIsPanel = 0x4000
ItemIsFocusScope = 0x8000
ItemIsHoverable = 0x10000
ItemStopsClickFocusPropagation = 0x20000
ItemSelfKeyFocus = 0x80000
ItemStopsMousePropagation = 0x100000
ItemClipsChildren = 0x200000
--};

--enum Curve
--{
Linear = 0
InQuad = 1
OutQuad = 2
InOutQuad = 3
OutInQuad = 4
InCubic = 5
OutCubic = 6
InOutCubic = 7
OutInCubic = 8
InQuart = 9
OutQuart = 10
InOutQuart = 11
OutInQuart = 12
InQuint = 13
OutQuint = 14
InOutQuint = 15
OutInQuint = 16
InSine = 17
OutSine =18
InOutSine = 19
OutInSine = 20
InExpo = 21
OutExpo = 22
InOutExpo = 23
OutInExpo = 24
InCirc = 25
OutCirc = 26
InOutCirc = 27
OutInCirc = 28
InElastic = 29
OutElastic = 30
InOutElastic = 31
OutInElastic = 32
InBack = 33
OutBack = 34
InOutBack = 35
OutInBack = 36
InBounce = 37
OutBounce = 38
InOutBounce = 39
OutInBounce = 40
InCurve = 41 
OutCurve = 42
SineCurve = 43
CosineCurve = 44
Custom = 45
NCurveTypes = 46
--};

--enum DeletionPolicy 
--{
KeepWhenStopped = 0
DeleteWhenStopped = 1
--};


--enum FontStyle
--{
FontStyleRegular = 0
FontStyleBold = 1
FontStyleItalic = 2
FontStyleBoldItalic = 3
FontStyleUnderline = 4
FontStyleStrikeout = 8
--};


--enum WidgetAttribute 
--{
WA_DeleteOnClose = 55
WA_DontShowOnScreen = 103
WA_ResizeAble = 133
WA_InnerToolTip = 134
--};

--enum ShowCommand
--{
SW_Hide = 0
SW_ShowNormal = 1
SW_Normal = 1
SW_ShowMinimized = 2
SW_ShowMaximized = 3
SW_Maximize = 3
SW_ShowNoActivate = 4
SW_Show = 5
SW_Minimize = 6
SW_ShowMinNoActive = 7
SW_ShowNA = 8
SW_Restore = 9
SW_ShowDefault = 10
SW_ForceMinimize = 11
SW_Max = 11
--};

-- THE SAME To AnchorLine
Invalid = 0x0
Left = 0x1
Right = 0x02
Top = 0x04
Bottom = 0x08
HCenter = 0x10
VCenter = 0x20
Baseline = 0x40

--enum FrameState
--{
SNormal = 0
SHover = 1
SPressDown = 2
SFocus = 3
SDisable = 4
SStateMax = 5
--};


--enum WinStyle
--{
AppWindow = 0
ToolWindow = 1
PopupWindow = 2
WinStyleMax = 3
--};

--enum DragPolicy
--{
NoDrag = 1
DragWindow = 2
AsParent = 3
--};

--enum GraphicsItemChange 
--{
ItemPositionChange = 0 
ItemVisibleChange = 1
ItemEnabledChange = 2 
ItemSelectedChange = 3
ItemParentChange = 4
ItemChildAddedChange = 5
ItemChildRemovedChange = 6
ItemPositionHasChanged = 7
ItemSceneChange = 8
ItemToolTipChange = 9
ItemToolTipHasChanged = 10
ItemFlagsChange = 11
ItemZValueChange = 12
ItemScenePositionHasChanged = 13
ItemGemoryChange = 14
--};

--enum Flow 
--{
LeftToRight = 0
TopToBottom  = 1
--};