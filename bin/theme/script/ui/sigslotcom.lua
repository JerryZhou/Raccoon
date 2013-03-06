
function SigSlotEmit(from, signal, ...)
    if from == nil then
        form = _G
    end
    Sigslot._emit_signal(from, signal, ...)
end

-- [[]]
function SigSlotConnect(from, signal, to, slot)
    if from == nil then
        form = _G
    end
    if to == nil then
        to = _G
    end
    return Sigslot._connect_slot(from , signal, to, slot)
end

-- [[]]
function SigSlotDisconnect(from, signal, to, slot)
    if from == nil then
        form = _G
    end
    if to == nil then
        to = _G
    end
    return Sigslot._disconnect_signal_slot(from, signal, to, slot)
end

-- [[]]
function SigSlotDisconnectSignal(from, signal)
    if from == nil then
        form = _G
    end
    Sigslot._disconnect_signal(from, signal)
end

-- [[]]
function SigSlotDisconnectSignals(from)
    if from == nil then
        form = _G
    end
    return Sigslot._disconnect_signal(from, nil)
end

-- [[]]
function SigSlotDisconnectSlot(to, slot)
    if to == nil then
        to = _G
    end
    return Sigslot._disconnect_slot(to, slot)
end

-- [[]]
function SigSlotDisconnectSlots(to)
    if to == nil then
        to = _G
    end
    return Sigslot._disconnect_slot(to, nil)
end

-- [[]]
function SigSlotDisconnectAll(obj)
    SigSlotDisconnectSignals(obj)
    SigSlotDisconnectSlots(obj)
end