/******************************************************************************
 * Copyright (C) 2016 Valerijus Drozdovas
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************/
#include "luamacaroon.h"
#include "macaroon.h"

int
l_mcr_newmacaroon(lua_State *L) {
    enum macaroon_returncode ret = 0;
    const char* location = luaL_checkstring(L, 1);
    const char* secure_key = luaL_checkstring(L, 2);
    const char* issuer_id = luaL_checkstring(L, 3);
    
    LuaMacaroon *lmacaroon = (LuaMacaroon*)lua_newuserdata(L, sizeof(LuaMacaroon));
    luaL_getmetatable(L, MACAROON_TBLNAME);
    lua_setmetatable(L, -2);
    
    lmacaroon->m = macaroon_create((const unsigned char*)location, strlen(location),
                                   (const unsigned char*)secure_key, strlen(secure_key),
                                   (const unsigned char*)issuer_id, strlen(issuer_id),
                                   &ret);
    
    if ( lmacaroon->m == NULL ) {
        luaL_error(L, "Macaroon create error code [%d]", ret);
        return 0;
    }
    
    return 1;
}

LuaMacaroon*
checkMacaroon(lua_State *L) {
    void *udata = luaL_checkudata(L, 1, MACAROON_TBLNAME);
    luaL_argcheck(L, udata != NULL, 1, "Macaroon expected");
    return (LuaMacaroon *)udata;
}

int
l_mcr_destroy(lua_State *L) {
    LuaMacaroon* m = checkMacaroon(L);
    macaroon_destroy(m->m);
    return 0;
}

int
l_mcr_to_string(lua_State *L) {
    enum macaroon_returncode ret = 0;
    LuaMacaroon* m = checkMacaroon(L);
    size_t ms_sz = macaroon_inspect_size_hint(m->m);
    char* ms = malloc(ms_sz);
    macaroon_inspect(m->m, ms, ms_sz, &ret);
    DEBUG("Inspect returns: %d-%d\n", ret, ret);
    lua_pushstring(L, ms);
    free(ms);
    return 1;
}

int
l_mcr_serialize(lua_State *L) {
    enum macaroon_returncode ret = 0;
    LuaMacaroon* m = checkMacaroon(L);
    size_t ms_sz = macaroon_serialize_size_hint(m->m);
    char* ms = malloc(ms_sz);
    macaroon_serialize(m->m, ms, ms_sz, &ret);
    lua_pushstring(L, ms);
    free(ms);
    // TODO add error handling from 'ret' result
    return 1;
}

int
l_mcr_deserialize(lua_State *L) {
    enum macaroon_returncode ret = 0;
    const char* mstr = luaL_checkstring(L, 1);
    if ( mstr == NULL ) {
        luaL_error(L, "Macaroon data required");
        return 0;
    }
    
    LuaMacaroon *lmacaroon = (LuaMacaroon*)lua_newuserdata(L, sizeof(LuaMacaroon));
    luaL_getmetatable(L, MACAROON_TBLNAME);
    lua_setmetatable(L, -2);
    
    lmacaroon->m = macaroon_deserialize(mstr, &ret);
    if ( lmacaroon->m == NULL ) {
        luaL_error(L, "Macaroon cannot be deserialized. Error [%d]", ret);
        return 0;
    }
    return 1;
}

int
l_mcr_add_caveat(lua_State *L) {
    enum macaroon_returncode ret = 0;
    LuaMacaroon* m = checkMacaroon(L);
    
    const char* caveat = luaL_checkstring(L, 2);
    if ( caveat == NULL ) {
        luaL_error(L, "Caveat required");
        return 0;
    }
    
    struct macaroon* mc = macaroon_add_first_party_caveat(m->m, (const unsigned char*)caveat, strlen(caveat), &ret);
    if ( mc == NULL ) {
        luaL_error(L, "Macaroon create error code [%d]", ret);
    } else {
        macaroon_destroy(m->m);
        m->m = mc;
    }
    
    return 0;
}


int
l_mcr_get_identifier(lua_State *L) {
    LuaMacaroon* m = checkMacaroon(L);
    size_t identifier_size = 0;
    const unsigned char* identifier = NULL;
    macaroon_identifier(m->m, &identifier, &identifier_size);
    if (identifier != NULL) {
        lua_pushlstring(L, (const char*)identifier, identifier_size);
        return 1;
    }
    return 0;
}

int
l_mcr_get_location(lua_State *L) {
    LuaMacaroon* m = checkMacaroon(L);
    size_t location_size = 0;
    const unsigned char* location = NULL;
    macaroon_location(m->m, &location, &location_size);
    if (location != NULL) {
        lua_pushlstring(L, (const char*)location, location_size);
        return 1;
    }
    return 0;
}






