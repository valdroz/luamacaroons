//
//  luamacaroon.h
//  luamacaroons
//
//  Created by Valerijus Drozdovas on 3/5/16.
//
//

#ifndef luamacaroon_h
#define luamacaroon_h


#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <macaroons.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


#define DEBUG_ENABLED 0

#define DEBUG(f, s ...) if (DEBUG_ENABLED) { printf("DEBUG: "); printf(f, s);}


#endif /* luamacaroon_h */
