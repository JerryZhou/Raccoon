-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: ui.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
local string = require("string")
local table = require("table")
local base = _G
module("Sigslot")
_sigslot_index = {}
_sigslot_slot_env = {}
_sigslot_signal_env = {}

local _helper_clear_signal_byslot = nil
local _helper_clear_slot_bysignal = nil

--[[ next is the run interfaces ]]
local function _helper_append_slot(slotList, to, slot)
	if not slotList or not slot then return false end
	local num = #slotList
	if num < 16 then
		slotList[num+1] = { ["object"] = to, ["slot"] = slot }
		return true
	end
	return false
end

local function _helper_is_slot_equal(sigslot, obj, slot)
    if sigslot and sigslot.object == obj and sigslot.slot == slot then
        return true
    end
    return false
end

--[[ next is the run interfaces ]]
local function _helper_remove_slot(slotList, to, slot)
	if not slotList or not slot then return false end
	local num = #slotList
	if num > 0 then
		for i=1, num, 1 do
			if _helper_is_slot_equal(slotList[i], to, slot) then
				if i == num then
					slotList[i] = nil
				else
					slotList[i] = slotList[num]
					slotList[num] = nil
				end
				return
			end
		end
	end
end

-- help function
local function _helper_get_sigslot_slotList(from, signal, alawys)
	local itemSlots =  _sigslot_slot_env[from]
	if not itemSlots and alawys then
		itemSlots = {}
		_sigslot_slot_env[from] = itemSlots
	elseif not itemSlots then
		return nil
	end
	local signalSlots = itemSlots[signal]
	if not signalSlots and alawys then
		signalSlots = {}
		itemSlots[signal] = signalSlots
	elseif not signalSlots  then
		return nil
	end
	return signalSlots
end

-- help function
_helper_clear_signal_byslot = function(from, signal, to, slot)
	local slotList = _helper_get_sigslot_slotList(from, signal, false)
	return _helper_remove_slot(slotList, to, slot)
end

-- help function
local function _helper_clear_signal(from, signal)
	local itemSlots =  _sigslot_slot_env[from]
	if itemSlots then
		local signalSlots = itemSlots[signal]
		if signalSlots then
		    local num = #signalSlots
            for i=1, num, 1 do
                local sigslot = signalSlots[i]
                _helper_clear_slot_bysignal(from, signal, sigslot.object, sigslot.slot)
            end
			itemSlots[signal] = nil
		end
	end
end

-- help function
local function _helper_clear_signals(from)
	local itemSlots =  _sigslot_slot_env[from]
	if itemSlots then
	    for signal, signalSlots in base.pairs(itemSlots) do
			if signalSlots then
				local num = #signalSlots
				for i=1, num, 1 do
					local sigslot = signalSlots[i]
					_helper_clear_slot_bysignal(from, signal, sigslot.object, sigslot.slot)
				end
			end
		end
		_sigslot_slot_env[from] = nil
	end
end

-- help
local function _helper_call_slot(slot, from,  ...)
    if slot then
        local obj = slot.object
        local slotcc = slot.slot
        if obj then
            if obj == base._G then
                obj[slotcc](from, ...)
            else
                obj[slotcc](obj, from, ...)
            end
        end
    end
end

-- help function
local function _helper_call_slotList(slotList, from, ...)	
    if not slotList then return false end
	local num = #slotList
	if num > 0 then
		for i=1, num, 1 do
			local slot = slotList[i]
			_helper_call_slot(slot, from, ...)
		end
	end
end

-- [[ ]]
local function _helper_append_signal(signalList, from, signal)
    if not signalList or not signal then return false end
	local num = #signalList
	if num < 16 then
		signalList[num+1] = { ["object"] = from, ["signal"] = signal }
		return true
	end
	return false
end

local function _helper_is_signal_equal(sigslot, obj, signal)
    if sigslot and sigslot.object == obj and sigslot.signal == signal then
        return true
    end
    return false
end

local function _helper_remove_signal(signalList, from, signal)
	if not signalList or not signal then return false end
	local num = #signalList
	if num > 0 then
		for i=1, num, 1 do
			if _helper_is_signal_equal(signalList[i], from, signal) then
				if i == num then
					signalList[i] = nil
				else
					signalList[i] = signalList[num]
					signalList[num] = nil
				end
				return
			end
		end
	end
end

-- help function
local function _helper_get_sigslot_signalList(to, slot, alawys)
	local itemSignals =  _sigslot_signal_env[to]
	if not itemSignals and alawys then
		itemSignals = {}
		_sigslot_signal_env[to] = itemSignals
	elseif not itemSignals then
		return nil
	end
	local signalSlots = itemSignals[slot]
	if not signalSlots and alawys then
		signalSlots = {}
		itemSignals[slot] = signalSlots
	elseif not signalSlots  then
		return nil
	end
	return signalSlots
end

-- help function
_helper_clear_slot_bysignal = function(from, signal, to, slot)
	local signalList = _helper_get_sigslot_slotList(to, slot, false)
	return _helper_remove_signal(signalList, from, signal)
end

-- help function
local function _helper_clear_slot(to, slot)
	local itemSignals =  _sigslot_signal_env[to]
    if itemSignals then
        local signalSlots = itemSignals[slot]
        if signalSlots then
            local num = #signalSlots
            for i=1, num, 1 do
                local sigslot = signalSlots[i]
                _helper_clear_signal_byslot(sigslot.object, sigslot.signal, to, slot)
            end
			itemSignals[slot] = nil
        end
    end
end

local function  _helper_clear_slots(to)
    local itemSignals =  _sigslot_signal_env[to]
    if itemSignals then
		for slot, signalSlots in base.pairs(itemSignals) do
			if signalSlots then
				local num = #signalSlots
				for i=1, num, 1 do
					local sigslot = signalSlots[i]
					_helper_clear_signal_byslot(sigslot.object, sigslot.signal, to, slot)
				end
			end
		end
		_sigslot_signal_env[to] = nil
	end
end

-- emit
function _emit_signal(item, signal, ...)
	local signalSlots = _helper_get_sigslot_slotList(item, signal, false)
	_helper_call_slotList(signalSlots, item, ...)
end

-- connect
function _connect_slot(from, signal, to, slot)
	local slotList = _helper_get_sigslot_slotList(from, signal, true)
	local success = _helper_append_slot(slotList, to, slot)
	if success then
	    local signalList = _helper_get_sigslot_signalList(to, slot, true)
	    success = _helper_append_signal(signalList, from, signal)
	end
	return success
end

-- disconnect
function _disconnect_signal_slot(from, signal, to, slot)
	_helper_clear_signal_byslot(from, signal, to, slot)
	_helper_clear_slot_bysignal(from, signal, to, slot)
end

-- disconnect
function _disconnect_signal(from, signal)
    if signal then
        _helper_clear_signal(from, signal)
    else
        _helper_clear_signals(from)
    end
end

-- disconnect
function _disconnect_slot(to, slot)
    if slot then
        _helper_clear_slot(to, slot)
    else
        _helper_clear_slots(to)
    end
end

function _unique_index()
    return _sigslot_index
end