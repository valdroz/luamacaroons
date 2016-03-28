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

LuaMacaroon*
checkMacaroon(lua_State *L);

#endif /* __macaroon_h */
