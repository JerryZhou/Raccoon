-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. set the text center in window
-- Author: Jeffery Zhou
-- RCS ID: $Id: 03.anchor.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

local mainWindow = FrameWindow
{
	size = { 380, 280 },
	
	childrens = function(p)
		local txt = GraphicsSimpleTextItem
		{
			text = "helloworld",
			x = 100, y = 100,
		}
		txt:d_anchors():d_setCenterIn(p)
	end,
}

mainWindow:d_show()