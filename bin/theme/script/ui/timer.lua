-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: timer.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
local string = require("string")
local table = require("table")
local base = _G

module("Timer")

local _timer_Proxy_wrap;
local _timer_proxy_wrap_connect;

local _timer_proxy_metatable = 
{
    d_start = function(self, ...)
        return base.Timer.start(base.rawget(self, "_timer_userdata"), ...)
    end,
    
    d_stop = function(self)
        return base.Timer.stop(base.rawget(self, "_timer_userdata"))
    end,
    
    d_isActive = function(self)
        return base.Timer.isActive(base.rawget(self, "_timer_userdata"))
    end,
    
    d_destroy = function(self)
        base.Timer.stop(base.rawget(self, "_timer_userdata"))
        self = nil
    end,
    
    d_setInterval = function(self, mesc)
        return base.Timer.setInterval(base.rawget(self, "_timer_userdata"), mesc)
    end,
    
    d_connect = function(self, handler)
        local weakRef = {}
        base.setmetatable(weakRef, {__mode="kv"})
        weakRef[1] = self
        return base.Timer.connect(base.rawget(self, "_timer_userdata"), _timer_proxy_wrap_connect(weakRef, handler))
    end,
    
    d_disconnect = function(self)
        return base.Timer.disconnect(base.rawget(self, "_timer_userdata"))
    end,
    
    d_id = function(self)
        return base.Timer.id(base.rawget(self, "_timer_userdata"))
    end,    
}

local _timer_metatable = 
{
    __index = _timer_proxy_metatable,
}

_timer_Proxy_wrap = function(id)
    local p = {}
    p["_timer_userdata"] = id
    base.setmetatable(p, _timer_metatable)
    return p
end

_timer_proxy_wrap_connect = function(weakRef, handler)
    return function(id, interval) handler(weakRef[1], interval) end
end

function new(mesc)
    local p = {}
    local stimer = base.Timer.create(mesc)
    p["_timer_userdata"] = stimer
    base.setmetatable(p, {__index = _timer_proxy_metatable})
    return p
end