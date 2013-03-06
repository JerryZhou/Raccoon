-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: animation.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
local string = require("string")
local table = require("table")
local base = _G

module("Animation")

function addAnimation(item, property, startv, endv, delta, curve)
	if not item then return 0 end
	if not property then return 0 end
	if not startv then return 0 end
	if not endv then return 0 end
	if not delta then
		delta = 1000
	end
	if not curve then
		curve = base.Dw.Linear
	end
	local anim = base.Animation.create()
	if not not anim and anim > 0 then
		base.Animation.setProperty(anim, property )
		base.Animation.setTarget(anim, item )
		base.Animation.setStep(anim, 0.0, startv)
		base.Animation.setStep(anim, 1.0, endv)
		base.Animation.setDuration(anim, delta)
		base.Animation.setEasingCurve(anim, curve)
	end
	return anim
end

function startAni(anim)
	if not not anim and anim > 0 then
		base.Animation.start(anim, base.Dw.DeleteWhenStopped)
	end
end
