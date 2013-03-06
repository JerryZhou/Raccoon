-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. add a animation to control window opacity 
-- Author: Jeffery Zhou
-- RCS ID: $Id: 06.animation.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

Assign.setAssign("SRes", "home:testdata/graphics_image")
local mainWindow = FrameWindow
{
	size = { 380, 280 },
	
	childrens = function(p)
		local pim = GraphicsPixmapItem
		{
			pixmap = "SRes:10.jpg",
		}
		pim:d_anchors():d_setCenterIn(p)
	end,
}

mainWindow:d_show()