-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. add a animation to control window opacity 
-- Author: Jeffery Zhou
-- RCS ID: $Id: 06.animation.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

local Template_OpacityWindow = 
{
	size = { 380, 280 },
	opacity = 0.5,
}

local mainWindow = FrameWindow(Template_OpacityWindow)
mainWindow:d_show()

local secondWindow = FrameWindow( Template_OpacityWindow, { width = 480,} )
secondWindow:d_show()