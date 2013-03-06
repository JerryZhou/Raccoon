Button_Template_ = 
{
	name = "button",
    size = {72, 28},
	focusPolicy = Dw.StrongFocus,
	
	onItemCreate = function(self, evt)
		self.background:d_setURL("dwgui:button.png")
		
		self.m_horizable = true
		self.m_content = GraphicsItem {parent = self}
		local anchors = self.m_content:d_anchors()
		anchors:d_setFill(self)
		
		self.m_offset = { 1, 1 }

		AddProperty(self, "text", self.d_setText, self.d_text)
		AddProperty(self, "icon", self.d_setIcon, self.d_icon)
		AddProperty(self, "textOnly", self.d_setTextOnly, self.d_textOnly)
		AddProperty(self, "iconOnly", self.d_setIconOnly, self.d_iconOnly)
		AddProperty(self, "horizable", self.d_setHorizable, self.d_horizable)
	end,
	
	onItemDestroy = function(self, evt)
	end,
	
	onMouseDown = function(self, evt)
		local anchors = self.m_content:d_anchors()
		anchors:d_setLeftMargin(self.m_offset[1])
		anchors:d_setTopMargin(self.m_offset[2])
		anchors:d_setRightMargin(-self.m_offset[1])
		anchors:d_setBottomMargin(-self.m_offset[2])
	end,
	
	onMouseUp = function(self, evt)
		local anchors = self.m_content:d_anchors()
		anchors:d_setLeftMargin(0)
		anchors:d_setTopMargin(0)
		anchors:d_setRightMargin(0)
		anchors:d_setBottomMargin(0)
	end,
	
	d_setBackground = function(self, url)
		local backgroundImg = Image.new(url)
		local imgSize = backgroundImg:d_size()
		self:d_setSize( {imgSize.width, imgSize.height} )
		self.background:d_setURL(url)
		backgroundImg = nil
	end,
	
	d_setIcon = function(self, url)
		if not self.m_icon then
			self.m_icon = GraphicsPixmapItem{ parent = self.m_content,}
			self.m_icon:d_setVisible(false)
			self.m_icon:d_setName("icon")
		end
		self.m_icon:d_setPixmap(url)
		self.m_icon:d_setVisible(true)
		self:d_setHorizable(self.m_horizable)
	end,

	d_icon = function(self)
		if not self.m_icon then
			return ""
		end
		return self.m_icon.pixmap
	end,
	
	d_setText = function(self, txt)
		if not self.m_text then
			self.m_text = GraphicsSimpleTextItem{ parent = self.m_content }
			self.m_text:d_setVisible(false)
			self.m_text:d_setName("text")
		end
		self.m_text:d_setText(txt)
		self.m_text:d_setVisible(true)
		self:d_setHorizable(self.m_horizable)
	end,

	d_text = function(self)
		if not self.m_text then
			return ""
		end
		return self.m_text.text
	end,
	
	d_setHorizable = function(self, h)
		self.m_horizable = h
		
		-- only text or only icon
		if not self.m_icon or not self.m_text then
			if self.m_icon then
				self.m_icon:d_clearAnchors();
				Anchor.setCenterIn(self.m_icon, AnchorLine(self.m_content))
			elseif self.m_text then
				self.m_text:d_clearAnchors();
				Anchor.setCenterIn(self.m_text, AnchorLine(self.m_content))
			end
			return
		end
		
		-- text and icon
		if not h then
			if self.m_icon then
				self.m_icon:d_clearAnchors()

				Anchor.setTop(self.m_icon, AnchorLine(self.m_content, Dw.Top))
				Anchor.setHCenter(self.m_icon, AnchorLine(self.m_content, Dw.HCenter))
			end

			if self.m_text then
				self.m_text:d_clearAnchors()

				if self.m_icon then
					Anchor.setTop(self.m_text, AnchorLine(self.m_icon, Dw.Bottom))
				else
					Anchor.setTop(self.m_text, AnchorLine(self.m_content, Dw.Top))
				end
				
				Anchor.setHCenter(self.m_text, AnchorLine(self.m_content, Dw.HCenter))
			end
		else
			if self.m_icon then
				self.m_icon:d_clearAnchors()

				Anchor.setLeft(self.m_icon, AnchorLine(self.m_content, Dw.Left))
				Anchor.setVCenter(self.m_icon, AnchorLine(self.m_content, Dw.VCenter))
			end
			
			if self.m_text then
				self.m_text:d_clearAnchors()

				if self.m_icon then
					Anchor.setLeft(self.m_text, AnchorLine(self.m_icon, Dw.Right))
				else
					Anchor.setLeft(self.m_text, AnchorLine(self.m_content, Dw.Left))
				end
				
				Anchor.setVCenter(self.m_text, AnchorLine(self.m_content, Dw.VCenter))
			end
		end
	end,

	d_horizable = function(self)
		return self.m_horizable
	end,
	
	d_setTextOnly = function(self)
		if self.m_icon then
			DestroyItem(self.m_icon)
			self.m_icon = nil
		end
		
		if not self.m_text then
			self.m_text = GraphicsSimpleTextItem{ parent = self.m_content }
			self.m_text:d_setVisible(false)
			self.m_text:d_setName("text")
		end
		self.m_text:d_clearAnchors();
		Anchor.setCenterIn(self.m_text, AnchorLine(self.m_content))
	end,

	d_textOnly = function(self)
		if self.m_icon then
			return false
		end
		if self.m_text then
			return true
		end
		return false
	end,
	
	d_setIconOnly = function(self)
		if self.m_text then
			DestroyItem(self.m_text)
			self.m_text = nil
		end

		if not self.m_icon then
			self.m_icon = GraphicsPixmapItem{ parent = self.m_content,}
			self.m_icon:d_setVisible(false);
			self.m_icon:d_setName("icon");
		end
		self.m_icon:d_clearAnchors();
		Anchor.setCenterIn(self.m_icon, AnchorLine(self.m_content))
	end,

	d_iconOnly = function(self)
		if self.m_text then
			return false
		end
		if self.m_icon then
			return true
		end
		return false
	end,
	
	d_adjustSize = function(self)
		if not self.m_icon or not self.m_text then return end
		
		if self.m_horizable then
			local anchorsIcon = self.m_icon:d_anchors()
			local anchorsText = self.m_text:d_anchors()

			local totalWidth = self.m_icon.width + self.m_text.width
			local totalMargin = anchorsIcon:d_leftMargin() + anchorsText:d_leftMargin() + anchorsText:d_rightMargin()
			self.width = totalWidth + totalMargin
		else
			local anchorsIcon = self.m_icon:d_anchors()
			local anchorsText = self.m_text:d_anchors()
			
			local totalHeight = self.m_icon.height + self.m_text.height
			local totalMargin = anchorsIcon:d_topMargin() + anchorsText:d_topMargin() + anchorsText:d_bottomMargin()
			self.height = totalHeight + totalMargin
		end
	end,
	
	d_setButtonMargin = function (self, left, left, right)
		if not self.m_icon or not self.m_text then return end
		if self.m_horizable then
			local anchorsIcon = self.m_icon:d_anchors()
			local anchorsText = self.m_text:d_anchors()
			
			anchorsIcon:d_setLeftMargin(left)
			anchorsText:d_setLeftMargin(left)
			anchorsText:d_setRightMargin(right)
		else
			local anchorsIcon = self.m_icon:d_anchors()
			local anchorsText = self.m_text:d_anchors()
			
			anchorsIcon:d_setTopMargin(left)
			anchorsText:d_setTopMargin(left)
			anchorsText:d_setBottomMargin(right)
		end
		self:d_adjustSize()
	end,
}

function Button(...)
	return WidgetItem(Button_Template_, ...)
end

--[[ closed button ]]
ClosedButton_Template_ = 
{
	name = "closedButton",
	
	onItemCreate = function(self, evt)	
		self:d_setIconOnly()	
		self:d_setBackground( "dwgui:close.png")
		self:d_setSize( { 28, 20} )
		
		Anchor.setTopMargin(self, -1)
		Anchor.setTop(self, AnchorLine(self.parent, Dw.Top))
		Anchor.setRightMargin(self, -2)
		Anchor.setRight(self, AnchorLine(self.parent, Dw.Right))
	end,
	
	onMouseClicked = function(self, evt)
		self.parent:d_close()
	end,
}

function ClosedButton(...)
	return WidgetItem(Button_Template_, ClosedButton_Template_, ...)
end

--[[ mini button ]]
MinimizeButton_Template_ =
{
	name = "minimizeButton",
	
	onItemCreate = function(self, evt)	
		self:d_setIconOnly()
		self:d_setBackground("dwgui:min.png")
		self:d_setSize( { 28, 20} )
		
		Anchor.setTopMargin(self, -1)
		Anchor.setTop(self, AnchorLine(self.parent, Dw.Top))
		Anchor.setRight(self, AnchorLine(self.parent:d_findChild("closedButton"), Dw.Left))
	end,
	
	onMouseClicked = function(self, evt)
		self.parent:d_show(Dw.SW_Minimize)
	end,
}

function MinimizeButton(...)
	return WidgetItem(Button_Template_, MinimizeButton_Template_, ...)
end