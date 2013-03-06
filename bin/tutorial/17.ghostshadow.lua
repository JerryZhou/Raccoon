require("script:ui/controls/button")
require("script:ui/controls/checkbox")
require("home:testdata/luabind_lua/combobox")

local mainWindow = FrameWindow
{
	size = {380, 280},
	backgroundImage = "home:testdata/mu/up.png.png",
	backgroundColor = { 0, 0, 0, 0},
	
	onItemCreate = function(self, evt)
		self:d_setAttribute(Dw.WA_ResizeAble, false)
		--self:d_setAttribute(Dw.WA_InnerToolTip, true)
	end,
	
	childrens = function(p)
		p.background:d_setMargins({0, 0, 0, 0})
		
		p.m_closedBtn = ClosedButton()		
		p.m_closedBtn:d_setButtonImage( "home:testdata/mu/btn_close_normal.png",
			"home:testdata/mu/btn_close_down.png",
			"home:testdata/mu/btn_close_down.png")
		p.m_closedBtn:d_setToolTips("closed\nNext Gamama Point Is 0.5,\nSOS!!\nSS\nSS")
			
		p.m_minimizeBtn = MinimizeButton()
		p.m_minimizeBtn:d_setButtonImage( "home:testdata/mu/btn_mini_normal.png",
			"home:testdata/mu/btn_mini_highlight.png",
			"home:testdata/mu/btn_mini_down.png")
			
		p.m_comboBox = ComboBox()
		local anchors = p.m_comboBox:d_anchors()
		anchors:d_setCenterIn(p)
		
		p.m_buttonList = RowPositioner()
		p.m_buttonList:d_setSpacing(5)
		local anchors = p.m_buttonList:d_anchors()
		anchors:d_setTopMargin(50)
		anchors:d_setTop(p, Dw.Top)
		anchors:d_setHCenter(p, Dw.HCenter)
		
		p.m_aniBtn01 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn01, "home:testdata/mu/aio_toobar_add.png")
		p.m_aniBtn02 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn02, "home:testdata/mu/aio_toobar_addhuman.png")
		p.m_aniBtn03 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn03, "home:testdata/mu/aio_toobar_app.png")
		p.m_aniBtn04 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn04, "home:testdata/mu/aio_toobar_block.png")
		p.m_aniBtn05 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn05, "home:testdata/mu/aio_toobar_game.png")
		
		p.m_aniBtn06 = Button{ parent = p.m_buttonList }
		p:d_setAnimationButton(p.m_aniBtn06, "home:testdata/mu/aio_toobar_game.png")
		p.m_aniBtn06:d_setText("Game")
		p.m_aniBtn06:d_setHorizable(true)
		p.m_aniBtn06:d_setButtonMargin( 2, 2, 2 )
		
		--p:d_addAnimation(p.m_minimizeBtn)
	end,
	
	d_addAnimation = function(self, btn)
		if true then return end
		local hoverEnter = function(self, evt)
			if self.m_org_normal then
				self:d_setImage(Dw.SNormal, self.m_org_normal)
			end
			local aniTarget = self.background
			aniTarget:d_clearAnimations()
			local curOpacity = aniTarget.opacity
			local ani = Animation.addAnimation(aniTarget, "opacity", curOpacity, 1.0, 100, Dw.InOutQuad)
			Animation.startAni(ani)
		end
		
		local hoverLeave = function(self, evt)
			local hoverImg = self:d_imageOf(Dw.SHover)
			self.m_org_normal = self:d_imageOf(Dw.SNormal)
			self:d_setImage(Dw.SNormal, hoverImg)
			local aniTarget = self.background
			aniTarget:d_clearAnimations()
			local curOpacity = aniTarget.opacity
			local ani = Animation.addAnimation(aniTarget, "opacity", 1.0, 0.0, 350, Dw.InOutQuad)
			Animation.startAni(ani)
		end
		
		local mouseDown = function(self, evt)
			if not self.m_content then return end
			local anchors = self.m_content:d_anchors()
			anchors:d_setLeftMargin(1)
			anchors:d_setTopMargin(1)
			anchors:d_setRightMargin(-1)
			anchors:d_setBottomMargin(-1)
		end
		
		local mouseUp = function(self, evt)
			if not self.m_content then return end
			local anchors = self.m_content:d_anchors()
			anchors:d_setLeftMargin(0)
			anchors:d_setTopMargin(0)
			anchors:d_setRightMargin(0)
			anchors:d_setBottomMargin(0)
		end
		
		local mouseClicked = function(self, evt)
			collectgarbage()
		end
		
		btn.onHoverEnter = hoverEnter
		btn.onHoverLeave = hoverLeave
		btn.onMouseDown = mouseDown
		btn.onMouseUp = mouseUp
		btn.onMouseClicked = mouseClicked
	end,
	
	d_setAnimationButton = function(self, btn, iconUrl)
		btn.background:d_setMargins( {2, 2, 2, 2} )
		btn:d_setImage(Dw.SNormal, "")
		btn:d_setImage(Dw.SHover, "home:testdata/mu/icon_background_hover.png")
		btn:d_setImage(Dw.SPressDown, "home:testdata/mu/icon_background_pressdown.png")
		btn:d_setIconOnly()
		btn:d_setIcon(iconUrl)
		
		local img = Image.new(iconUrl)
		if img then
			local imgSize = Point.add( img:d_size(), {4, 4})
			btn:d_setSize(imgSize)
		end
		
		self:d_addAnimation(btn)
	end,
}

local ani = Animation.addAnimation(mainWindow, "width", 0, 380, 1000)			
Animation.startAni(ani)

local ani = Animation.addAnimation(mainWindow, "opacity", 0.0, 1.0, 1500, Dw.InOutQuad)			
Animation.startAni(ani)

mainWindow:d_show()
