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
#include "verifier.h"
#include "macaroon.h"

static LuaMacaroonVerifier *
check_verifier(lua_State *L) {
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
    verifier->gen_func = NULL;
    verifier->ctr = 'V';
    
    if ( verifier->v == NULL ) {
        luaL_error(L, "Create Verifier failed");
        return 0;
    }
    
    return 1;
}

int
l_mcrv_destroy(lua_State *L) {
    LuaMacaroonVerifier* v = check_verifier(L);
    macaroon_verifier_destroy(v->v);
    if (v->gen_func != NULL) {
        free(v->gen_func);
    }
    return 0;
}

int
l_mcrv_to_string(lua_State *L) {
    LuaMacaroonVerifier* v = check_verifier(L);
    lua_pushfstring(L, "Verifier[%s]", v->gen_func);
    return 1;
}

int
general_check(void* vv, const unsigned char* pred, size_t pred_sz) {
    LuaMacaroonVerifier* v = (LuaMacaroonVerifier*)vv;
    if (v->ctr == 'V' && v->gen_func != NULL) {
        lua_getglobal(v->state, v->gen_func);
        lua_pushlstring(v->state, (const char *)pred, pred_sz);
        if (lua_pcall(v->state, 1, 1, 0) != 0) {
            luaL_error(v->state, "Error calling callback function : %s",
                  lua_tostring(v->state, -1));
            return -1;
        }
        if (!lua_isnumber(v->state, -1)) {
            luaL_error(v->state, "Callback function must return a number");
        }
        int result = lua_tointeger(v->state, -1);
        lua_pop(v->state, 1);  /* pop returned value */
        return result;
    }
    return 0;
}

int
l_mcrv_verify(lua_State *L) {
    enum macaroon_returncode ret = 0;
    
    LuaMacaroonVerifier* v = check_verifier(L);
    v->state = L;
    void *udata = luaL_checkudata(L, 2, MACAROON_TBLNAME);
    luaL_argcheck(L, udata != NULL, 2, "Macaroon expected");
    LuaMacaroon* m = (LuaMacaroon *)udata;
    
    
    const char* secret = luaL_checkstring(L, 3);
    if ( secret == NULL ) {
        luaL_error(L, "Secret key required");
        return 0;
    }
    
    struct macaroon* discarge = NULL;
    size_t discharge_size = 0;
    
    int rc = macaroon_verify(v->v, m->m, (const unsigned char*)secret, strlen(secret), &discarge, discharge_size, &ret);
    if ( rc == 0 ) {
        lua_pushboolean(L, 1);
    } else if ( ret == MACAROON_NOT_AUTHORIZED ) {
        lua_pushboolean(L, 0);
    } else {
        luaL_error(L, "Macaroon verification failed. Error code [%d]", ret);
    }
    
    return 1;
}

int
l_mcrv_verify_satisfy_exact(lua_State *L) {
    enum macaroon_returncode ret = 0;
    
    LuaMacaroonVerifier* v = check_verifier(L);
    
    const char* predicate = luaL_checkstring(L, 2);
    if ( predicate == NULL ) {
        luaL_error(L, "Predicate required");
        return 0;
    }
    
    int rc = macaroon_verifier_satisfy_exact(v->v, (const unsigned char*)predicate, strlen(predicate), &ret);
    if ( rc < 0 ) {
        luaL_error(L, "Macaroon verify exact failed. Error code [%d]", ret);
    }
    
    return 0;
}

int
l_mcrv_verify_satisfy_general(lua_State *L) {
    enum macaroon_returncode ret = 0;
    LuaMacaroonVerifier* v = check_verifier(L);
    const char* gen_func = luaL_checkstring(L, 2);
    if ( gen_func == NULL ) {
        luaL_error(L, "Lua function name required");
    } else {
        size_t name_len = strlen(gen_func) + 1;
        if ( macaroon_verifier_satisfy_general(v->v, general_check, v, &ret) < 0 ) {
            luaL_error(L, "Macaroon verify exact failed. Error code [%d]", ret);
            return 0;
        }
        v->gen_func = malloc(name_len);
        strcpy(v->gen_func, gen_func);
    }
    return 0;
}


