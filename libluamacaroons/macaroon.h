//
//  macaroon.h
//  luamacaroons
//
//  Created by Valerijus Drozdovas on 3/27/16.
//
//

#ifndef __macaroon_h
#define __macaroon_h

#define MACAROON_TBLNAME "Macaroons.Macaroon"

typedef struct {
    struct macaroon *m;
} LuaMacaroon;

int
l_mcr_newmacaroon(lua_State *L);

int
l_mcr_destroy(lua_State *L);

int
l_mcr_to_string(lua_State *L);

int
l_mcr_serialize(lua_State *L);

int
l_mcr_deserialize(lua_State *L);

int
l_mcr_add_caveat(lua_State *L);

int
l_mcr_get_identifier(lua_State *L);

int
l_mcr_get_location(lua_State *L);

#endif /* __macaroon_h */
