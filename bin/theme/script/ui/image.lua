local string = require("string")
local table = require("table")
local base = _G
module("Image")
local _image_proxy_metatable = 
{
	d_size = function(self)
		return base.Image.size(base.rawget(self, "_img_userdata"))
	end,
	
	d_isNull = function(self)
		return base.Image.isNull(base.rawget(self, "_img_userdata"))
	end,
	
	d_uniqueKey = function(self)
		return base.Image.uniqueKey(base.rawget(self, "_img_userdata"))
	end,
	
	d_path = function(self)
		return base.Image.path(base.rawget(self, "_img_userdata"))
	end,
	
	d_frameCount = function(self)
		return base.Image.frameCount(base.rawget(self, "_img_userdata"))
	end,
	
	d_curActiveFrame = function(self)
		return base.Image.curActiveFrame(base.rawget(self, "_img_userdata"))
	end,
	
	d_setActiveFrame = function(self, idx)
		base.Image.setActiveFrame(base.rawget(self, "_img_userdata"), idx) 
	end,
	
	d_nextInterval = function(self)
		base.Image.nextInterval(base.rawget(self, "_img_userdata"))
	end,
	
	d_shrinkCaches = function(self, num)
		if not num then
			num = -1
		end
		base.Image.shrinkCaches(num)
	end,
	
	d_hasCachedOf = function(self, url)
		return base.Image.hasCachedOf(url)
	end,
}

local _image_proxy_metatable_inner = 
{
	__index = _image_proxy_metatable,
}

function _image_proxy(img)
	if img and base.type(img) == "userdata" then
		local p = {}
		p["_img_userdata"] = img
		base.setmetatable(p, _image_proxy_metatable_inner)
		return p
	end
	return img
end 

function new(url)
	local img = base.Image.create(url)
	return _image_proxy(img)
end