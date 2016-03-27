//
//  verifier.h
//  luamacaroons
//
//  Created by Valerijus Drozdovas on 3/27/16.
//
//

#ifndef __verifier_h
#define __verifier_h

typedef struct {
    struct macaroon_verifier *v;
} LuaMacaroonVerifier;

#define VERIFIER_TBLNAME "Macaroons.Verifier"

int
l_mcrv_newverifier(lua_State *L);

int
l_mcrv_destroy(lua_State *L);

int
l_mcrv_to_string(lua_State *L);

#endif /* __verifier_h */
