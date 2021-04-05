local fh
local result
function commandinfo(cmd)
    fh = io.popen(cmd)
    result = fh:read("*all")
    fh:close()
    return tostring(result)
end

