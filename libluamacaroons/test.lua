print("Start")
-- local path = "/lualibmacaroons/libluamacaroons/libluamacaroons.so"
local path = "./libluamacaroons.so"
local init_macaroons = assert(package.loadlib(path, "luaopen_macaroons"))
init_macaroons()
M = macaroon.new("http://comcast.com","secret","codebig2")
print(M:serialize())
print("----------------------------------")
M.addcaveat(M, "123123234123")
print(M:serialize())
print("End")
