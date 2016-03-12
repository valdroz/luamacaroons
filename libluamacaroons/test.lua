print("Start")
local path = "./libluamacaroons.so"
local init_macaroons = assert(package.loadlib(path, "luaopen_macaroons"))
init_macaroons()

m = macaroon.new("http://location.com","secret","issuer_id")
print(m:serialize())
print(m)

print("----------------------------------")
m:addcaveat("id = 123")
print(m:serialize())
print(m)

print("End")
