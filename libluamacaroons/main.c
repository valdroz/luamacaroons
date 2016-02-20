#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <macaroons.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static int l_macaroon_create (lua_State *L) {
    enum macaroon_returncode ret = 0;
    const char* location = luaL_checkstring(L, 1);
    const char* secure_key = luaL_checkstring(L, 2);
    const char* issuer_id = luaL_checkstring(L, 3);
    struct macaroon* m1 = 0L;
    m1 = macaroon_create(location, strlen(location),
                    secure_key, strlen(secure_key),
                    issuer_id, strlen(issuer_id),
                    &ret);

    if ( m1 == NULL ) {
        luaL_error(L, "Macaroon create error code [%d]", ret);
    } else {
        size_t ms_sz = macaroon_serialize_size_hint(m1);
        char* ms = malloc(ms_sz);
        macaroon_serialize(m1, ms, ms_sz, &ret);
        macaroon_destroy(m1);
        lua_pushstring(L, ms);
        free(ms);
    }
    return 1;
}

static int l_macaroon_add_caveat (lua_State *L) {
    enum macaroon_returncode ret = 0;
    char* mstr = luaL_checkstring(L, 1);
    if ( mstr == NULL ) {
        luaL_error(L, "Macaroon data required");
        return 0;
    }

    char* caveat = luaL_checkstring(L, 2);
    if ( caveat == NULL ) {
        luaL_error(L, "Caveat required");
        return 0;
    }

    struct macaroon* m = macaroon_deserialize(mstr, &ret);
    if ( m == NULL ) {
        luaL_error(L, "Macaroon cannot be desrailized. Error [%d]", ret);
        return 0;
    }
    struct macaroon* mc = macaroon_add_first_party_caveat(m, caveat, strlen(caveat), &ret);
    macaroon_destroy(m);
    if ( mc == NULL ) {
        luaL_error(L, "Macaroon create error code [%d]", ret);
    } else {
        size_t ms_sz = macaroon_serialize_size_hint(mc);
        char* ms = malloc(ms_sz);
        macaroon_serialize(mc, ms, ms_sz, &ret);
        macaroon_destroy(mc);
        lua_pushstring(L, ms);
        free(ms);
    }
    return 1;
}


static const struct luaL_Reg lua_export [] = {
    {"macaroon_add_ceveat", l_macaroon_add_caveat},
    {"macaroon_create", l_macaroon_create},
    {NULL, NULL}
};


int luaopen_macaroons (lua_State *L) {
    luaL_register(L, "macaroons", lua_export);
    return 1;
}

//int main(void)
//{
//    lua_State *L = luaL_newstate();
//    luaL_openlibs(L);
//    luaopen_macaroons(L);
//    luaL_dofile(L, "test.lua");
//    return 0;
//}
