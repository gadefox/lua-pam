## lua-pam
A module for lua to use PAM.

## Complile
```shell
configure [--debug]
make
```

The file `libpam.so` is installed into `/usr/local/lib/lua/5.3`.
Make sure the `liblua-dev` and `libpam0g-dev` packages are installed

## Usage
```lua
local pam = require("libpam")
local result = pam.auth(password)
print(result)
```
