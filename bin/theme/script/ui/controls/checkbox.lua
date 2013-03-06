require("script:ui/controls/button")

Dw.UnChecked = false
Dw.Checked = true

CheckBox_Images = 
{
	[1] = 
	{
		"dwgui:checkbox_default_disable.png",--disable 1
		"dwgui:checkbox_default_pressdown.png", -- press 2 
		"dwgui:checkbox_default_pressdown.png", -- focus 3
		"dwgui:checkbox_default_hover.png", -- hover 4
		"dwgui:checkbox_default_normal.png", -- normal 5
	},
	
	[2] = 
	{
		"dwgui:checkbox_selected_disable.png",--disable 1
		"dwgui:checkbox_selected_pressdown.png", -- press 2 
		"dwgui:checkbox_selected_pressdown.png", -- focus 3
		"dwgui:checkbox_selected_hover.png", -- hover 4
		"dwgui:checkbox_selected_normal.png", -- normal 5
	}
}

CheckBox_Template_ = 
{
	name = "button",
	
	onItemCreate = function(self, evt)
		self.m_pressed = false
		self.m_hover = false
		self.m_disable = false
		self.m_focus = false
		self.background.visible = false
		self.focusable = false
		self.m_state = false
		
		Button_Template_.d_setIcon(self, "dwgui:checkbox_default_normal.png")
		Button_Template_.d_setText(self, "sure")
		
		self:d_setChecked(Dw.UnChecked)
	end,
	
	onItemDestroy = function(self, evt)
		Button_Template_.onItemDestroy(self, evt)
	end,
	
	d_setIcon = function(self, url)
		Button_Template_.d_setIcon(self, url)
		self:d_adjustSize()
	end,
	
	d_setText = function(self, txt)
		Button_Template_.d_setText(self, txt)
		self:d_adjustSize()
	end,
	
	d_setChecked = function(self, state)
		if self.m_state == state then return end
		
		self.m_state = state
		self:d_updateIcon()
	end,
	
	d_state = function(self)
		return self.m_state
	end,
	
	onMouseClicked = function(item, evt)
		item:d_setChecked(not item.m_state)
	end,
	
	onMouseDown = function(item, evt)
		item.m_pressed = true
		item:d_updateIcon()
	end,
	
	onMouseUp = function(item, evt)
		item.m_pressed = false
		item:d_updateIcon()
	end,
	
	onHoverEnter = function(item, evt)
		item.m_hover = true
		item:d_updateIcon()
	end,
	
	onHoverLeave = function(item, evt)
		item.m_hover = false
		item:d_updateIcon()
	end,
	
	onFocusIn = function(item, evt)
		item.m_focus = true
		item:d_updateIcon()
	end,
	
	onFocusOut = function(item, evt)
		item.m_focus = false
		item:d_updateIcon()
	end,
	
	-- diable (1), press (2), focus (3), hover (4), normal (5)
	d_currentIndex = function(self)
		if self.m_disable then
			return 1
		elseif self.m_pressed then
			return 2
		elseif self.m_focus then
			return 3
		elseif self.m_hover then
			return 4
		else
			return 5
		end
	end,
	
	d_updateIcon = function(self)
		local idx = self:d_currentIndex()
		local se = 1
		if self:d_state() then
			se = 2
		end
		local url = CheckBox_Images[se][idx]
		self:d_setIcon(url)
	end,
}

function CheckBox(...)
	return WidgetItem(Button_Template_, CheckBox_Template_, ...)
end