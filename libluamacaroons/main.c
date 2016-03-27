#include "luamacaroon.h"
#include "verifier.h"
#include "macaroon.h"


// Show registration of class
static const luaL_Reg lua_exp_macaroonFuncs[] = {
    {"new", l_mcr_newmacaroon},
    {"deserialize", l_mcr_deserialize},
    {NULL, NULL}
};

static const luaL_Reg lua_exp_macaroonMethods[] = {
    {"serialize", l_mcr_serialize},
    {"addcaveat", l_mcr_add_caveat},
    {"identifier", l_mcr_get_identifier},
    {"location", l_mcr_get_location},
    {"__tostring", l_mcr_to_string},
    {"__gc", l_mcr_destroy},
    {NULL, NULL}
};

// Show registration of class
static const luaL_Reg lua_exp_verifierFuncs[] = {
    {"new", l_mcrv_newverifier},
    {NULL, NULL}
};

static const luaL_Reg lua_exp_verifierMethods[] = {
    {"__tostring", l_mcrv_to_string},
    {"__gc", l_mcrv_destroy},
    {NULL, NULL}
};

int
luaopen_macaroons (lua_State *L) {
    luaL_newmetatable(L, MACAROON_TBLNAME);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    luaL_openlib(L, NULL, lua_exp_macaroonMethods, 0);

    luaL_openlib(L, "Macaroon", lua_exp_macaroonFuncs, 0);

    luaL_newmetatable(L, VERIFIER_TBLNAME);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    luaL_openlib(L, NULL, lua_exp_verifierMethods, 0);
    
    luaL_openlib(L, "MacaroonVerifier", lua_exp_verifierFuncs, 0);
    
    return 2;
}

//int main(void)
//{
//    lua_State *L = luaL_newstate();
//    luaL_openlibs(L);
//    luaopen_macaroons(L);
//    luaL_dofile(L, "test.lua");
//    return 0;
//}
