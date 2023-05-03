local libpam = require("libpam")
local result = libpam.auth("pwd")
print(result)
