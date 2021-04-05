#include <string>
#include <iostream>
#ifdef __cplusplus
	extern "C" {
#include "lua.h"
#include <lauxlib.h>
#include <lualib.h>
}
#else
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

using namespace std;

static inline std::string command(const std::string & cmd){
    lua_State *L =NULL;
    L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L,"../script/loablib.lua");
    lua_getglobal(L,"commandinfo");
    lua_pushstring(L,cmd.data());
    lua_pcall(L,1,1,0);
    std::string result = lua_tostring(L,-1);
    lua_close(L);
    return result;
}


