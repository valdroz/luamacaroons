print("Start")
local path = "./libluamacaroons.so"
local init_macaroons = assert(package.loadlib(path, "luaopen_macaroons"))
init_macaroons()

local m = macaroon.new("http://location.com","secret","issuer_id")
print(m:serialize())
print(m)

print("----------------------------------")
m:addcaveat("id = 123")
local mser = m:serialize()
print("Serialized macaroon: " .. mser)
print(m)
print("----------------------------------")
print("Deserializing macaroon")
local m2 = macaroon.deserialize(mser)
m2:addcaveat("expired in 30sec")
print("End result: " .. tostring(m2))

print("End")
