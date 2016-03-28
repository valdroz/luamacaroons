
function satisfy_general_func(pred)
	print("Satisfy : " .. tostring(pred))
	return 0;
end

print("Start")
local path = "./libluamacaroons.so"
-- local path = "./libluamacaroons.dylib"
local init_macaroons = assert(package.loadlib(path, "luaopen_macaroons"))
init_macaroons()

local mv = MacaroonVerifier.new()

local m = Macaroon.new("http://location.com","secret","issuer_id")
print(m:serialize())
-- print(m)

local vresult = mv:verify(m, "secret")
print("Verification with valid secret result : " .. tostring(vresult))


print("----------------------------------")
m:addcaveat("id = 123")
m:addcaveat("am > 1000")
local mser = m:serialize()
mv:satisfyexact("id = 123")
mv:satisfyexact("am > 1000")
mv:satisfygeneral("satisfy_general_func")
vresult = mv:verify(m, "secret")
print("Verification with valid secret result : " .. tostring(vresult))

print("----------------------------------")
print("Deserializing macaroon")
local m2 = Macaroon.deserialize(mser)
m2:addcaveat("expired in 30sec")
print("End result: " .. tostring(m2))

local m_id = m2:identifier()
local m_loc = m2:location()

print("Macaroon Identifier :\"" .. m_id .. "\"")
print("Macaroon Location   :\"" .. m_loc .. "\"")
print("----------------------------------")
local mv2 = MacaroonVerifier.new()
mv2:satisfygeneral("satisfy_general_func")
print("Instance : " .. tostring(mv2) .. "; Verification result: " .. tostring(mv2:verify(m2, "secret")))


print("End")


