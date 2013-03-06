require("script:ui/controls/button")
require("script:ui/controls/checkbox")

Font.setDefaultName("Microsoft YaHei")
Font.setDefaultSize(12)

Assign.setAssign("sciptHome", "home:testdata/mu")

local mainWindow = FrameWindow
{
	size = {379, 280},
	
	onItemCreate = function(self, evt)
		self:d_setAttribute(Dw.WA_ResizeAble, false)
	end,
	
	onMouseClicked = function(item, evt)	
		if toShowHideWindow:d_isValid() then
			if item.m_isShowWin then
				toShowHideWindow:d_hide()
			else
				toShowHideWindow:d_show()
			end
			item.m_isShowWin = not item.m_isShowWin
		end
	end,
	
	childrens = function(p)	
	
		p.m_closedBtn = ClosedButton()
		
		p.m_btn0 = Button()
		p.m_btn0:d_setText("中文")
		p.m_btn0:d_setIcon("sciptHome:aio_toobar_game.png")
		p.m_btn0:d_setHorizable(true)
		p.m_btn0.height = 40
		p.m_btn0:d_anchors():d_setCenterIn(p)
		p.m_btn0.m_text.font = { style = Dw.FontStyleBold }
		p.m_btn0:d_adjustSize()
		
		p.m_btn1 = Button()
		p.m_btn1:d_setText("中文")
		p.m_btn1:d_setIcon("sciptHome:aio_toobar_game.png")
		p.m_btn1:d_setHorizable(true)
		p.m_btn1.height = 40
		p.m_btn1.pos = {20, 20}
		p.m_btn1.enabled = false
		p.m_btn1:d_adjustSize()
	end,
}

mainWindow:d_show()