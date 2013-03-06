-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. add a animation to control window opacity 
-- Author: Jeffery Zhou
-- RCS ID: $Id: 06.animation.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

local mainWindow = FrameWindow
{
	size = { 380, 280 },
	
	onItemCreate = function(self, evt)
		local anis = Animation.addAnimation(
			self, "opacity", 0.0, 1.0, 2500, Dw.InOutQuad)
		Animation.startAni(anis)
	end,
}

mainWindow:d_show()