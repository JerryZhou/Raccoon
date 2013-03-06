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
		GridPositioner
		{
			spacing = 2,
			columns = 6,
			
			childrens = function(p)
				local anchors = p:d_anchors()
				anchors:d_setCenterIn(p.parent)
				
				for i = 10, 30, 1 do
					GraphicsPixmapItem { pixmap = "SRes:" .. i .. ".jpg" }
				end
			end,
		}
	end,
}

mainWindow:d_show()