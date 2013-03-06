require("script:ui/controls/button")
require("script:ui/controls/checkbox")

local mainWindow = FrameWindow
{
	size = {380, 280},
	
	onItemCreate = function(self, evt)
		self:d_setAttribute(Dw.WA_ResizeAble, false)
		self.m_ut = Timer.new(80);
		self.m_ut:d_connect(function(t, i) self:d_update() end);
		self.m_ut:d_start()
	end,
	
	onItemDestroy = function(self, evt)
		self.m_ut = nil
	end,
	
	
	childrens = function(p)
		local content = GraphicsItem
		{	
			childrens = function(p)
				p:d_anchors():d_setFill(p.parent)
				
				p.m_up = GraphicsPixmapItem{ pixmap = "home:testdata/mu/up.png.png", height = 130, }
				p.m_mid = GraphicsPixmapItem{ pixmap = "home:testdata/mu/mid.png.png",}
				p.m_down = GraphicsPixmapItem{ pixmap = "home:testdata/mu/down.png.png", height = 38, }
				
				local upAnchors = p.m_up:d_anchors()
				upAnchors:d_setTop(p, Dw.Top)
				upAnchors:d_setLeft(p, Dw.Left)
				upAnchors:d_setRight(p, Dw.Right)
				
				local downAnchors = p.m_down:d_anchors()
				downAnchors:d_setLeft(p, Dw.Left)
				downAnchors:d_setRight(p, Dw.Right)
				downAnchors:d_setBottom(p, Dw.Bottom)
				
				local midAnchors = p.m_mid:d_anchors()
				midAnchors:d_setTop(p.m_up, Dw.Bottom)
				midAnchors:d_setLeft(p, Dw.Left)
				midAnchors:d_setRight(p, Dw.Right)
				midAnchors:d_setBottom(p.m_down, Dw.Top)
			end,
		}
		
		p.m_closedBtn = ClosedButton()
			
		p.m_minimizeBtn = MinimizeButton()
		
		p.m_title = GraphicsPixmapItem{ pixmap="home:testdata/mu/Main_Title.png",}
		local anchors = p.m_title:d_anchors()
		anchors:d_setLeftMargin(6)
		anchors:d_setLeft(p, Dw.Left)
		anchors:d_setTopMargin(4)
		anchors:d_setTop(p, Dw.Top)
		
		--[[ Top ]]
		p.m_banner = GraphicsPixmapItem { parent = content.m_up, pixmap="home:testdata/mu/YY.png", }
		local anchors = p.m_banner:d_anchors()
		anchors:d_setCenterIn(content.m_up)
		
		--[[ Down ]]
		p.m_loginBtn = Button { parent = content.m_down, }
		p.m_loginBtn:d_setTextOnly()
		p.m_loginBtn:d_setText("Enter")
		local anchors = p.m_loginBtn:d_anchors()
		anchors:d_setRightMargin(7)
		anchors:d_setRight(content.m_down, Dw.Right)
		anchors:d_setVCenter(content.m_down, Dw.VCenter)
		
		p.m_multiAccountBtn = Button { parent = content.m_down, }
		p.m_multiAccountBtn:d_setIcon("home:testdata/mu/main_tabbtn_rightnormal.png")
		p.m_multiAccountBtn:d_setText("Acounts")
		p.m_multiAccountBtn:d_setHorizable(true)
		p.m_multiAccountBtn:d_setButtonMargin(3, 3, 3)
		local anchors = p.m_multiAccountBtn:d_anchors()
		anchors:d_setLeftMargin(7)
		anchors:d_setLeft(content.m_down, Dw.Left)
		anchors:d_setVCenter(content.m_down, Dw.VCenter)
		
		p.m_settingBtn = Button { parent = content.m_down, }
		p.m_settingBtn:d_setTextOnly()
		p.m_settingBtn:d_setText("Setting")
		local anchors = p.m_settingBtn:d_anchors()
		anchors:d_setLeftMargin(7)
		anchors:d_setLeft(p.m_multiAccountBtn, Dw.Right)
		anchors:d_setVCenter(content.m_down, Dw.VCenter)
		
		--[[ Mid ]]
		p.m_headGraphics = GraphicsFrameItem { parent = content.m_mid, size = {83, 83}, }
		local anchors = p.m_headGraphics:d_anchors()
		anchors:d_setLeftMargin(20)
		anchors:d_setLeft(content.m_mid, Dw.Left)
		anchors:d_setVCenter(content.m_mid, Dw.VCenter)
		
		p.m_headMask = Button { parent = p.m_headGraphics, focusable = false, }
		p.m_headMask:d_setBackground("dwgui:edit.png")
		local anchors = p.m_headMask:d_anchors()
		anchors:d_setMargins(-2)
		anchors:d_setFill(p.m_headGraphics)
		
		p.m_mid_mid = GraphicsItem
		{
			parent = content.m_mid,
			eventHandler = "DefaultItemEventHandler",
			
			onItemCreate = function(item, evt)
				local anchors = item:d_anchors()
				anchors:d_setTop(p.m_headGraphics, Dw.Top)
				anchors:d_setBottom(p.m_headGraphics, Dw.Bottom)
				anchors:d_setLeft(p.m_headGraphics, Dw.Right)
				anchors:d_setLeftMargin(10)
				anchors:d_setRight(content.m_mid, Dw.Right)
			end,
			
			childrens = function(pp)
				pp.m_name = WidgetItemOf("LineEdit", {width=190,})
				pp.m_passport = WidgetItemOf("LineEdit", {width=190,})
				pp.m_checkRememberPassort = CheckBox()
				pp.m_checkRememberPassort:d_setButtonMargin(0, 1, 0)
				pp.m_checkRememberPassort:d_setText("Remember")
				pp.m_autoLogin = CheckBox()
				pp.m_autoLogin:d_setButtonMargin(0, 1, 0)
				pp.m_autoLogin:d_setText("Auto Login")
				pp.m_register = GraphicsSimpleTextItem { text = "Register", font = { color = {39, 134, 228}, style=Dw.FontStyleUnderline}}
				pp.m_retrieve = GraphicsSimpleTextItem { text = "Forgot", font = { color = {39, 134, 228}, style=Dw.FontStyleUnderline}}
				
				local anchors = pp.m_passport:d_anchors()
				anchors:d_setTopMargin(8)
				anchors:d_setTop(pp.m_name, Dw.Bottom)
				
				local anchors = pp.m_autoLogin:d_anchors()
				anchors:d_setTopMargin(4)
				anchors:d_setTop(pp.m_passport, Dw.Bottom)
				anchors:d_setRightMargin(-1)
				anchors:d_setRight(pp.m_passport, Dw.Right)
				
				local anchors = pp.m_checkRememberPassort:d_anchors()
				anchors:d_setRightMargin(3)
				anchors:d_setRight(pp.m_autoLogin, Dw.Left)
				anchors:d_setVCenter(pp.m_autoLogin, Dw.VCenter)
				
				local anchors = pp.m_register:d_anchors()
				anchors:d_setLeftMargin(5)
				anchors:d_setLeft(pp.m_name, Dw.Right)
				anchors:d_setVCenter(pp.m_name, Dw.VCenter)
				
				local anchors = pp.m_retrieve:d_anchors()
				anchors:d_setLeftMargin(5)
				anchors:d_setLeft(pp.m_passport, Dw.Right)
				anchors:d_setVCenter(pp.m_passport, Dw.VCenter)
			end,
		}
	end,
}

local ani = Animation.addAnimation(mainWindow, "width", 0, 380, 1000)			
Animation.startAni(ani)

mainWindow:d_show()
mainWindow.m_loginBtn:d_setFocus()