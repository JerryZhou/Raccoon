local math = require("math")
local table = require("table")
module("Point")

-- add (x1, y1) + (x2, y2) = (x1 + x2, y1 + y2)
function add(lfs, rfs)
	local t = {}
	for i=1, #lfs, 1 do
		t[i] = lfs[i] + rfs[i]
	end
	return t
end

-- sub (x1, y1) - (x2, y2) = (x1 - x2, y1 - y2)
function sub(lfs, rfs)
	local t = {}
	for i=1, #lfs, 1 do
		t[i] = lfs[i] - rfs[i]
	end
	return t
end

-- scale (x1, y1) * a = (a*x1, a*y1)
function scale(v, factor)
	local t = {}
	for i=1, #v, 1 do
		t[i] = v[i] * factor
	end
	return t
end

-- minus (x1, y1)  = (-x1, -y1)
function minus(v)
	return scale(v, -1)
end

--[[
	THE NEXT IS THE VECTOR INTERFACES 
]]

-- lengthSquare(x1, y1) = x1 *x1 + y1 * y1
function lengthSquare(v)
	local t = 0
	for i=1, #v, 1 do
		t = t + v[i] * v[i]
	end
	return t
end

-- length (x1, y1) = math.sqrt ( lengthSquare (x1, y1) )
function length(v)
	return math.sqrt(lengthSquare(v))
end

-- |a||b|cos<a,b> 
-- dot (x1, y1) . (x2, y2) = x1*x2 + y1*y2
function dot(lfs, rfs)
	local t = 0
	for i=1, #lfs, 1 do
		t = t + lfs[i] * rfs[i]
	end
	return t
end

function angle(lfs, rfs)
	local lfsLength = length(lfs)
	local rfsLength = length(rfs)
	if lfsLength == 0 or rfsLength == 0 then return 0 end
	local dotLR = dot(lfs, rfs)
	local cosLR = dotLR / (lfsLength * rfsLength)
	local arc = math.acos(cosLR)
	return math.deg(arc)
end

-- |c|=|a¡Áb|=|a||b|sin<a,b> 
-- cross (x1, y1, z1) * (x2, y2, z2) = { y1z2 - z1y2, z1x2 - z2x1, x1y2 - x2y1)
function cross(lfs, rfs)
	return { lfs[2]*rfs[3] - lfs[3]*rfs[2], 
	lfs[3]*rfs[1] - lfs[1]*rfs[3], 
	lfs[1]*rfs[2] - lfs[2]*rfs[1] }
end

function normalize(lfs)
    local len = length(lfs)
    if len and len > 0 then
        local t = {}
        for i=1, #lfs, 1 do
            t[i] = lfs[i]/len
        end
        return t
    end
    return lfs
end