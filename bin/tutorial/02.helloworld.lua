-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. add a text to window
-- Author: Jeffery Zhou
-- RCS ID: $Id: helloworld.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

local mainWindow = FrameWindow
{
	childrens = function(p)
		GraphicsSimpleTextItem
		{
			text = "helloworld",
			x = 100, y = 100
		}
	end,
}

mainWindow:d_show()