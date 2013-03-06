-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: anchor.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
local string = require("string")
local table = require("table")
local base = _G

module("Anchor")

-- THE SAME To AnchorLine
Invalid = 0x0
Left = 0x1
Right = 0x02
Top = 0x04
Bottom = 0x08
HCenter = 0x10
VCenter = 0x20
Baseline = 0x40

-- wrap a anchor line 
AnchorLine = function(p, l)
	if l == nil then
		l = Invalid
	end
	return { item = base.GraphicsItemId(p), line = l }
end

local _anchor_proxy_metatable = 
{
	d_setMargins = function(self, m)
		base.Anchor.setMargins(self[1], m)
	end,
	
	d_margins = function(self)
		return base.Anchor.margins(self[1])
	end,
	
	d_setLeft = function(self, anchor)
		base.Anchor.setLeft(self[1], anchor)
	end,
	
	d_left = function(self)
		return base.Anchor.leftAnchor(self[1])
	end,
	
	d_setLeftMargin = function(self, m)
		base.Anchor.setLeftMargin(self[1], m)
	end,
	
	d_leftMargin = function(self)
		return base.Anchor.leftMargin(self[1])
	end,
	
	d_setRight = function(self, anchor)
		base.Anchor.setRight(self[1], anchor)
	end,
	
	d_right = function(self)
		return base.Anchor.rightAnchor(self[1])
	end,
	
	d_setRightMargin = function(self, m)
		base.Anchor.setRightMargin(self[1], m)
	end,
	
	d_rightMargin = function(self)
		return base.Anchor.rightMargin(self[1])
	end,
	
	d_setTop = function(self, anchor)
		base.Anchor.setTop(self[1], anchor)
	end,
	
	d_top = function(self)
		return base.Anchor.topAnchor(self[1])
	end,
	
	d_setTopMargin = function(self, m)
		base.Anchor.setTopMargin(self[1], m)
	end,
	
	d_topMargin = function(self)
		return base.Anchor.topMargin(self[1])
	end,
	
	d_setBottom = function(self, anchor)
		base.Anchor.setBottom(self[1], anchor)
	end,
	
	d_bottom = function(self)
		return base.Anchor.bottomAnchor(self[1])
	end,
	
	d_setBottomMargin = function(self, m)
		base.Anchor.setBottomMargin(self[1], m)
	end,
	
	d_bottomMargin = function(self)
		return base.Anchor.bottomMargin(self[1])
	end,
	
	d_setHCenter = function(self, anchor)
		base.Anchor.setHCenter(self[1], anchor)
	end,
	
	d_hCenter = function(self)
		return base.Anchor.hCenterAnchor(self[1])
	end,
	
	d_setHCenterOffset = function(self, m)
		base.Anchor.setHCenterOffset(self[1], m)
	end,
	
	d_hCenterOffset = function(self)
		return base.Anchor.hCenterOffset(self[1])
	end,
	
	d_setVCenter = function(self, anchor)
		base.Anchor.setVCenter(self[1], anchor)
	end,
	
	d_vCenter = function(self)
		return base.Anchor.vCenterAnchor(self[1])
	end,
	
	d_setVCenterOffset = function(self, m)
		base.Anchor.setVCenterOffset(self[1], m)
	end,
	
	d_vCenterOffset = function(self)
		return base.Anchor.vCenterOffset(self[1])
	end,
	
	d_setFill = function(self, fill)
		base.Anchor.setFill(self[1], AnchorLine(fill, Invalid))
	end,
	
	d_fill = function(self)
		return base.Anchor.fillAnchor(self[1])
	end,
	
	d_setCenterIn = function(self, center)
		base.Anchor.setCenterIn(self[1], AnchorLine(center, Invalid))
	end,
	
	d_centerIn = function(self)
		return base.Anchor.centerInAnchor(self[1])
	end,
}

local _anchor_proxy_metatable_inner = 
{
	__index = _anchor_proxy_metatable,
}

function _anchor_proxy(item)
	local p = {}
	base.setmetatable(p, _anchor_proxy_metatable_inner)
	p[1] = item
	return p
end