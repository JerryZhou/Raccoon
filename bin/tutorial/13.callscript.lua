-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. add a animation to control window opacity 
-- Author: Jeffery Zhou
-- RCS ID: $Id: 06.animation.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
require("script:ui/controls/button")

local mainWindow = FrameWindow
{
	size = { 380, 280 },
	
	childrens = function(p)	
		ClosedButton()
	end,
}

call_by_cplusplus = function()
	local changeColorTimer = Timer.new(200)
	changeColorTimer:d_connect( 
		function(t, interval) 
			if mainWindow and mainWindow:d_isValid() then
				mainWindow.backgroundColor = { 
				math.random(0, 255), 
				math.random(0, 255), 
				math.random(0, 255) }
				
				if interval < 1000 then
					t:d_setInterval(interval + 10)
				end
			else
				t:d_destroy()
			end
		end)
	changeColorTimer:d_start()
	
	mainWindow.m_updateColorTimer = changeColorTimer
end

mainWindow:d_show()