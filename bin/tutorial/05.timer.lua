-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script Tutorial. start the timer to change the window
-- Author: Jeffery Zhou
-- RCS ID: $Id: 05.timer.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

local mainWindow = FrameWindow
{
	size = { 380, 280 },
	
	onItemCreate = function(self, evt)
		local changeColorTimer = Timer.new(200)
		changeColorTimer:d_connect( 
			function(t, interval) 
				self.backgroundColor = { 
				math.random(0, 255), 
				math.random(0, 255), 
				math.random(0, 255) }
			end)
		changeColorTimer:d_start()
		
		self.m_updateColorTimer = changeColorTimer
	end,
}

mainWindow:d_show()