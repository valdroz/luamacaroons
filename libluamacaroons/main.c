#include "luamacaroon.h"

typedef struct {
    struct macaroon *m;
} LuaMacaroon;

static int l_mcr_new(lua_State *L) {
    enum macaroon_returncode ret = 0;
    const char* location = luaL_checkstring(L, 1);
    const char* secure_key = luaL_checkstring(L, 2);
    const char* issuer_id = luaL_checkstring(L, 3);
    
    LuaMacaroon *lmacaroon = (LuaMacaroon*)lua_newuserdata(L, sizeof(LuaMacaroon));
    luaL_getmetatable(L, "Macaroons.macaroon");
    lua_setmetatable(L, -2);
    
    lmacaroon->m = macaroon_create((const unsigned char*)location, strlen(location),
                         (const unsigned char*)secure_key, strlen(secure_key),
                         (const unsigned char*)issuer_id, strlen(issuer_id),
                         &ret);
    
    if ( lmacaroon->m == NULL ) {
        luaL_error(L, "Macaroon create error code [%d]", ret);
    }
    
    return 1;
}

static LuaMacaroon *checkMacaroon (lua_State *L) {
    void *udata = luaL_checkudata(L, 1, "Macaroons.macaroon");
    luaL_argcheck(L, udata != NULL, 1, "'Macaroons' expected");
    return (LuaMacaroon *)udata;
}

static int l_mcr_destroy(lua_State *L) {
    LuaMacaroon* m = checkMacaroon(L);
//    printf("DEBUG: Macaroon destroy\n");
    macaroon_destroy(m->m);
    return 0;
}

static int l_mcr_to_string(lua_State *L) {
    enum macaroon_returncode ret = 0;
    LuaMacaroon* m = checkMacaroon(L);
    size_t ms_sz = macaroon_inspect_size_hint(m->m);
    char* ms = malloc(ms_sz);
    printf("DEBUG: inspect size: %d\n", (int)ms_sz);
    macaroon_inspect(m->m, ms, ms_sz, &ret);
    printf("DEBUG: inspect ret code: %d\n", ret);
    printf("DEBUG: inspect string: %s\n", ms);
    //lua_pushstring(L, ms);
    lua_pushstring(L, "todo here");
    free(ms);
    return 1;
}

static int l_mcr_serialize(lua_State *L) {
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

static int l_mcr_deserialize(lua_State *L) {
    return 0;
}

static int l_mcr_add_caveat (lua_State *L) {
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



// ===========================

static int l_macaroon_create (lua_State *L) {
    enum macaroon_returncode ret = 0;
    const char* location = luaL_checkstring(L, 1);
    const char* secure_key = luaL_checkstring(L, 2);
    const char* issuer_id = luaL_checkstring(L, 3);
    struct macaroon* m1 = 0L;
    m1 = macaroon_create((const unsigned char*)location, strlen(location),
                         (const unsigned char*)secure_key, strlen(secure_key),
                         (const unsigned char*)issuer_id, strlen(issuer_id),
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
    const char* mstr = luaL_checkstring(L, 1);
    if ( mstr == NULL ) {
        luaL_error(L, "Macaroon data required");
        return 0;
    }

    const char* caveat = luaL_checkstring(L, 2);
    if ( caveat == NULL ) {
        luaL_error(L, "Caveat required");
        return 0;
    }

    struct macaroon* m = macaroon_deserialize(mstr, &ret);
    if ( m == NULL ) {
        luaL_error(L, "Macaroon cannot be desrailized. Error [%d]", ret);
        return 0;
    }
    struct macaroon* mc = macaroon_add_first_party_caveat(m, (const unsigned char*)caveat, strlen(caveat), &ret);
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



int macaroon_to_string (lua_State *L) {
//    lua_pushfstring(L, "array(%d)", a->size);
    return 1;
}


//static const struct luaL_Reg lua_export [] = {
//    {"macaroon_add_ceveat", l_macaroon_add_caveat},
//    {"macaroon_create", l_macaroon_create},
//    {NULL, NULL}
//};


// Show registration of class
static const luaL_Reg lua_exp_macaroolFuncs[] = {
    {"new", l_mcr_new},
    {"deserialize", l_mcr_deserialize},
    {NULL, NULL}
};

static const luaL_Reg lua_exp_macaroolMethods[] = {
    {"serialize", l_mcr_serialize},
    {"addcaveat", l_mcr_add_caveat},
    {"__tostring", l_mcr_to_string},
    {"__gc", l_mcr_destroy},
    {NULL, NULL}
};


int luaopen_macaroons (lua_State *L) {
//    luaL_register(L, "macaroons", lua_export);
//    
//    luaL_register(L, "Macaroon", lua_exp_macaroolFuncs);
//    lua_pushvalue(L,-1);
//    lua_setfield(L, -2, "__index");
    
    luaL_newmetatable(L, "Macaroons.macaroon");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    luaL_openlib(L, NULL, lua_exp_macaroolMethods, 0);
    luaL_openlib(L, "macaroon", lua_exp_macaroolFuncs, 0);
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
