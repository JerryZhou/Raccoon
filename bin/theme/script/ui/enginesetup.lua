-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: enginesetup.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
dwLoadLib = function(url, entry)
    local path = Assign.localPath(url)
    local libEntry = package.loadlib(path, entry)
    if libEntry then
        return libEntry()
    end
	return  nil
end

dwAddRequireSearchPath = function(p) 
    if dw_append_search_path then
        dw_append_search_path(p)
    else
        package.path = package.path .. ";" .. p ;
    end
end

dwAddRequireSearchUrl = function(p)
    local localPath = Assign.localPath(p)
    if not localPath or string.len(localPath) < 2 then return nil end
    local sp = string.sub(localPath, -1, 1)
    if sp == ":" or sp == "/" or sp == "\\" then
        localPath = localPath .. "?.lua"
    else
        localPath = localPath .. "/?.lua"
    end
    dwAddRequireSearchPath(localPath)
end