module("Variant")
function CreateVariant(t, v)    return { type = t, value = v }end
-- DwVariant--enum Type --{Invalid = 0EChar = 1EUChar = 2EBool = 3EInt = 4EUInt = 5EDouble = 6EFloat = 7EInt64 = 8EUInt64 =9EPtr = 10ERectF = 11ERect = 12EPointF = 13EPoint = 14ESizeF = 15ESize = 16EString = 17--};EReal = 6 -- DwReal