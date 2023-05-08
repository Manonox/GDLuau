local function is_good_key(s)
    return string.match(s, "^[%a_][%w_]*$") ~= nil
end

local cache
local function _printtable(t, i, printcustom)
    if cache[t] then return end
    cache[t] = true
    local prefix = string.rep("\t", i)
    for k, v in pairs(t) do
        if type(k) ~= "string" then
            k = "[" .. k .. "]"
        elseif not is_good_key(k) then
            k = "[\\"" .. k .. "\\"]"
        end
        
        if type(v) ~= "table" then
            if type(v) == "string" then
                v = "\\"" .. v .. "\\""
            end
            if type(v) == "vector" then
                v = "vector(" .. tostring(v) .. ")"
            end
            printcustom(prefix .. k .. " = ", v, ",")
        else
            printcustom(prefix .. k .. " = {")
            _printtable(v, i + 1, printcustom)
            printcustom(prefix .. "},")
        end
    end
end

local function printtable(t, printcustom)
    cache = {}
    print("{")
    _printtable(t, 1, printcustom or print)
    print("}")
end

return printtable
