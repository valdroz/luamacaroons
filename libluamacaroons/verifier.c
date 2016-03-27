//
//  verifier.c
//  luamacaroons
//
//  Created by Valerijus Drozdovas on 3/27/16.
//
//

#include "luamacaroon.h"
#include "verifier.h"

static LuaMacaroonVerifier *
checkVerifier(lua_State *L) {
    void *udata = luaL_checkudata(L, 1, VERIFIER_TBLNAME);
    luaL_argcheck(L, udata != NULL, 1, "'Verifier' expected");
    return (LuaMacaroonVerifier *)udata;
}


int
l_mcrv_newverifier(lua_State *L) {
    LuaMacaroonVerifier *verifier = (LuaMacaroonVerifier*)lua_newuserdata(L, sizeof(LuaMacaroonVerifier));
    luaL_getmetatable(L, VERIFIER_TBLNAME);
    lua_setmetatable(L, -2);
    
    verifier->v = macaroon_verifier_create();
    
    if ( verifier->v == NULL ) {
        luaL_error(L, "Create Verifier failed");
        return 0;
    }
    
    return 1;
}

int
l_mcrv_destroy(lua_State *L) {
    LuaMacaroonVerifier* v = checkVerifier(L);
    macaroon_verifier_destroy(v->v);
    return 0;
}

int
l_mcrv_to_string(lua_State *L) {
    LuaMacaroonVerifier* v = checkVerifier(L);
    lua_pushstring(L, "Verifier");
    return 1;
}
