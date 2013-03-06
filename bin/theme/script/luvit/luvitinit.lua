
-- Bootstrap require system
local native = require('uv_native')

process = {
  execPath = native.execpath(),
  cwd = getcwd,
  argv = argv
}

local Emitter = require('core').Emitter
local timer = require('timer')
local env = require('env')
local constants = require('constants')
local uv = require('uv')
local utils = require('utils')

setmetatable(process, {
  __index = function (table, key)
    if key == "title" then
      return native.getProcessTitle()
    else
      return Emitter[key]
    end
  end,
  __newindex = function (table, key, value)
    if key == "title" then
      return native.setProcessTitle(value)
    else
      return rawset(table, key, value)
    end
  end,
  __pairs = function (table)
    local key = "title"
    return function (...)
      if key == "title" then
        key = next(table)
        return "title", table.title
      end
      if not key then return nil end
      local lastkey = key
      key = next(table, key)
      return lastkey, table[lastkey]
    end
  end
})

-- Replace lua's stdio with luvit's
-- leave stderr using lua's blocking implementation
process.stdin = uv.createReadableStdioStream(0)
process.stdout = uv.createWriteableStdioStream(1)
process.stderr = uv.createWriteableStdioStream(2)

-- clear some globals
-- This will break lua code written for other lua runtimes
_G.print = utils.print
_G.p = utils.prettyPrint
_G.debug = utils.debug

-- Add a way to exit programs cleanly
local exiting = false
function process.exit(exit_code)
  if exiting == false then
    exiting = true
    process:emit('exit', exit_code or 0)
  end
  exitProcess(exit_code or 0)
end

function process:addHandlerType(name)
  local code = constants[name]
  if code then
    native.activateSignalHandler(code)
  end
end

function process:missingHandlerType(name, ...)
  if name == "error" then
    error(...)
  elseif name == "SIGINT" or name == "SIGTERM" then
    process.exit()
  end
end

function process.nextTick(callback)
  timer.setTimeout(0, callback)
end

-- Add global access to the environment variables using a dynamic table
process.env = setmetatable({}, {
  __pairs = function (table)
    local keys = env.keys()
    local index = 0
    return function (...)
      index = index + 1
      local name = keys[index]
      if name then
        return name, table[name]
      end
    end
  end,
  __index = function (table, name)
    return env.get(name)
  end,
  __newindex = function (table, name, value)
    if value then
      env.set(name, value, 1)
    else
      env.unset(name)
    end
  end
})

--Retrieve PID
process.pid = native.getpid()

-- Copy date and time over from lua os module into luvit os module
local OLD_OS = require('os')
local OS_BINDING = require('os_binding')
package.loaded.os = OS_BINDING
package.preload.os_binding = nil
package.loaded.os_binding = nil
OS_BINDING.date = OLD_OS.date
OS_BINDING.time = OLD_OS.time
OS_BINDING.clock = OLD_OS.clock


-- Ignore sigpipe and exit cleanly on SIGINT and SIGTERM
-- These shouldn't hold open the event loop
if OS_BINDING.type() ~= "win32" then
  native.activateSignalHandler(constants.SIGPIPE)
  native.activateSignalHandler(constants.SIGINT)
  native.activateSignalHandler(constants.SIGTERM)
end

local traceback = require('debug').traceback

-- This is called by all the event sources from C
-- The user can override it to hook into event sources
function eventSource(name, fn, ...)
  local args = {...}
  return assert(xpcall(function ()
    return fn(unpack(args))
  end, traceback))
end