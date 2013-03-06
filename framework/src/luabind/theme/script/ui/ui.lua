-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: ui.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
local string = require("string")
local table = require("table")
local base = _G

module("ui")

--[[ the parent index when construct the parent ]]
_ui_env = {}
local _ui_env_ref_parent = 0
local _ui_env_ref_current = 1
local _ui_env_ref_proxy = 2
local _ui_env_ref_propertys = 3
_ui_env[_ui_env_ref_parent] = {}
_ui_env[_ui_env_ref_current] = {}
_ui_env[_ui_env_ref_proxy] = {}
_ui_env[_ui_env_ref_propertys] = {}
base.setmetatable(_ui_env[_ui_env_ref_proxy], { __mode = "v", } )

-- push parent
local function _ui_env_push_parent( p )
	local n = base.table.getn(_ui_env[_ui_env_ref_parent])
	--base.print ("Push Parent: ", p, n)
	_ui_env[_ui_env_ref_parent][n+1] = p
end

-- pop parent
local function _ui_env_pop_parent()
	local n = base.table.getn(_ui_env[_ui_env_ref_parent])
	if n > 0 then
		--base.print ("Pop Parent: ", n)
		_ui_env[_ui_env_ref_parent][n] = nil
	end
end

-- current parent
local function _ui_evn_parent()
	local n = base.table.getn(_ui_env[_ui_env_ref_parent])
	if n > 0 then
		--base.print ("Current Parent: ", _ui_env[_ui_env_ref_parent][n], n)
		return _ui_env[_ui_env_ref_parent][n]
	end
end

-- register ref to proxy
local function _ui_env_register_proxy(id, p)
	_ui_env[_ui_env_ref_proxy][id] = p
end

local function _ui_env_unRegister_proxy(id)
	if not _ui_env[_ui_env_ref_proxy][id] then return end
	_ui_env[_ui_env_ref_proxy][id] = nil
end

local function _ui_env_destroy_proxy(id)
	if not not base.GUI then
		local _ui_lib = base.GUI
		if not not id and not not _ui_lib.destroyItem then
		_ui_lib.destroyItem(id)
		end
	end
	
	_ui_env_unRegister_proxy(id)
end

-- find ref to proxy
local function _ui_env_proxy_of(id)
	return _ui_env[_ui_env_ref_proxy][id]
end

-- get the id of proxy
local function _ui_env_proxy_id(p)
	if not p then return -1 end
	return base.rawget(p, "_ui_id")
end

-- wrap a this to the function
local function _ui_proxy_event_function( proxy, v)
	-- weak ref to proxy
	-- local SlefProxy = {}
	-- base.setmetatable(SlefProxy, { __mode="kv",} )
	-- SlefProxy[1] = proxy
	return function(id, ...) return v(_ui_env_proxy_of(id), ...) end
end

-- wrap a call to the org
local function _ui_proxy_call_function( org, k)
	if not not base.GUI then
		local _ui_lib = base.GUI
		return function(self, ...) return _ui_lib.call( self, k, ...) end
	else
		return nil
	end
end

--[[ the index listener for the item op ]]
local _ui_index = {}
base["_ui_index"] = _ui_index

local _ui_proxy_mt = 
{
	__index = function( t, k)
		-- access the original table
		-- base.print ("access", t, k)
		local org = base.rawget(t, _ui_index)
		local orgK = org[k]
		if not not orgK then
			return orgK
		end
		
		-- script member data
		local kstr = base.tostring(k)
		if string.len(kstr) >= 1 and string.sub(kstr, 1, 2) == "m_" then
			return nil
		end
		
		-- access the call
		local kstr = base.tostring(k)
		if not not kstr and string.len(kstr)>2 and string.sub(kstr, 1, 2) == "d_" then
			local k_c = string.sub(kstr, 3)
			local d_call =  _ui_proxy_call_function(org, k_c)
			if not not d_call then
				-- speed the meta call
				org[k] = d_call
				return d_call
			end
		end
		
		-- access the property
		if not not base.GUI then
			local _ui_lib = base.GUI
			return _ui_lib.propertyOf( org["_ui_id"], k)
		else
			return nil
		end
	end,
	
	__newindex = function( t, k, v)
		-- update original table
		-- base.print ("update", t, k, v)	
		local org = base.rawget(t, _ui_index)

		-- script member data
		local kstr = base.tostring(k)
		if string.len(kstr) >= 1 and string.sub(kstr, 1, 2) == "m_" then
			org[k] = v
			return
		end
		
		-- function
		if base.type(v) == "function" then
			-- connect to org table
			if k == "childrens" then
				v(t)
				return
			end
			
			--local wrapV = _ui_proxy_event_function(t, v)
			--v = wrapV	
			base.rawset(org, k, v)
			
			-- connect the event
			local kstr = base.tostring(k)
			if not not kstr then
				if string.len(kstr) > 2 and string.sub(kstr, 1, 2) == "on" then
					if not not base.Event then
						local _event_lib = base.Event
						if not not _event_lib.connect then
							_event_lib.connect( org["_ui_id"], kstr, v)
						end
					end
				end
			end
			
			if v then
				return
			end
		-- child 
		--elseif base.type(v) == "table" then
			--local isProxy = base.rawget(v, "_ui_id")
			--if not not isProxy then
				--base.rawset(org, k, v)
				--return
			--end
		end

		if not not base.GUI then
			local _ui_lib = base.GUI
			return _ui_lib.setProperty( org["_ui_id"], k, v)
		else
			org[k] = v
		end
	end,
	
	__metatable = "not your business"
}

local function _ui_proxy_new_wrap(_control)
	local proxy = {}
	local args = {}
	base.setmetatable(args, { __mode="kv",} )
	
	proxy._ui_id = _control
	args["_ui_id"] = _control
	_ui_env_register_proxy(_control, proxy)
	
	args._ui_proxy = proxy
	proxy[_ui_index] = args
	base.setmetatable(proxy, _ui_proxy_mt)
	return proxy
end

local function _ui_proxy_listener(args)
	local proxy = {}
	proxy._ui_child = {}
	base.print ("Create Item:  ", args._ui_type, "Parent: ", args._ui_parent)
	if not not base.GUI then
		local _ui_lib = base.GUI
		local _control = _ui_lib.createItem( args._ui_type, _ui_env_proxy_id(args._ui_parent) )
		if not _control then
			return
		else
			proxy._ui_id = _control
			args["_ui_id"] = _control
			_ui_env_register_proxy(_control, proxy)
		end
	end
	-- NB! the args's __mode is "kv"
	args._ui_proxy = proxy
	proxy[_ui_index] = args
	base.setmetatable(proxy, _ui_proxy_mt)
	return proxy
end

-- need to confirm the construct in order
_ui_env[_ui_env_ref_propertys] = { 
"name", "parent", "boundingRect", "pos", "x", "y", "rect",
"eventHandler", "onItemCreate", "onItemDestroy" ,
}


local function _ui_proxy_assign(control, args)
	
	local seq = _ui_env[_ui_env_ref_propertys]
	local hasSeted = {}
	
	-- init the first
	for i, k in base.ipairs(seq) do
		base.print ( "Assign :", i, k)
		local v = args[k]
		if not not v then
			control[k] = v
			-- kick the already set
			hasSeted[k] = true
		end
	end
	
	-- assign propertys
	for k, v in base.pairs(args) do
		base.print ( "Assign :", k, v, base.type(k), base.type(v))
		if not hasSeted[k] then
			control[k] = v
		end
	end
	
	-- call create event 
	if not not base.Event then
		local org = base.rawget(control, _ui_index)
		local _event_lib = base.Event
		if not not _event_lib.call then
			_event_lib.call( org["_ui_id"], "onItemCreate", v)
		end
	end
	
	return control
end

-- THE INTERFACE FOR USER TO EXTEND THE UI LIB
-- create item
function _ui_widget_create(t, ...)
	-- require the env
	local p = _ui_evn_parent()
	
	-- base.print ("current parent: ", p)
	-- create item
	local args = { _ui_type = t, _ui_parent = p,};
	base.setmetatable(args, { __mode="kv",} )
	local control = _ui_proxy_listener( args )
	if not control then return end
	
	-- set the parent and child
	base.rawset(control, "_ui_parent", p)
	if not not p then
		local pChild = base.rawget(p, "_ui_child")
		pChild[base.table.getn(pChild)+1] = control 
	end
	
	-- set the propertys
	_ui_env_push_parent( control )
	for i = 1, arg.n, 1 do 
		control = _ui_proxy_assign(control, arg[i])
	end
	_ui_env_pop_parent()
	
	-- link the parent and child
	if not not p then
		local cn = control["name"]
		if not not cn then
			-- base.print ("Link Childrens", p, control, cn)
			base.rawset(p, cn, control)
		end
	end
	
	return control
end

-- get id of proxy
function _ui_widget_get_id(p)
	return _ui_env_proxy_id(p)
end

-- find it by id
function _ui_widget_get_proxy(id)
	local proxy = _ui_env_proxy_of(id)
	if not proxy then
		proxy = _ui_proxy_new_wrap(id)
	end
	return proxy
end

-- destry widget
function _ui_widget_destroy_of_p(proxy)
	if not proxy then return end
	
	local pParent = base.rawget(proxy, "_ui_parent")
	local pName = proxy["name"]
	if not not pParent and not not pName then
		local linkedP = base.rawget(pParent, pName)
		if linkedP == proxy then
			base.rawset(proxy, pName, nil)
		end
	end
	
	local id = _ui_widget_get_id(proxy)
	_ui_env_destroy_proxy(id)
	proxy = nil
end

function _ui_widget_destroy_of_id(id)
	local proxy = _ui_widget_get_proxy(id)
	_ui_widget_destroy_of_p(proxy)
end

function _ui_widget_unRegister_of_id(id)
	_ui_env_unRegister_proxy(id)
end
