#pragma once

#include "LuaMeshDataStruct.h"

// lua head files, in the C style
#ifdef __cplusplus
extern "C"
{
#endif
// includes
#include "..\..\lua\lua.h"
#include "..\..\lua\lauxlib.h"
#include "..\..\lua\lualib.h"
// includes
#ifdef __cplusplus
}
#endif

// compiled lua dll
#pragma comment(lib, "lua.lib")


// get pointer (in the userData) from the lua_State
#define checkMeshData(L) \
	reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>\
	(luaL_checkudata(L, 1, "LoadAssets.MeshData"))->pointer

// get pointerContainer (in the userData) from the lua_State
#define checkConainer(L) \
	reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>\
	(luaL_checkudata(L, 1, "LoadAssets.MeshData"))

#ifdef __cplusplus
extern "C"
{
#endif
	int DLL_API luaopen_MeshData(lua_State * L);
#ifdef __cplusplus
}
#endif

// create a new MeshData.
static int lua_newMeshData(lua_State * L);

// just show the data for test
static int lua_showMeshData(lua_State * L);

// just show the data for test
static int lua_getDetail(lua_State * L);

// add positioni to the buffer 
static int lua_addPosition(lua_State * L);

// add normal to the buffer
static int lua_addNormal(lua_State * L);

// add tangentU to the buffer
static int lua_addTangentU(lua_State * L);

// add textureCoord to the buffer
static int lua_addTexC(lua_State * L);

// add a vertex using the index of the previous added positions/normals.....
// here we must pass 4 integer as respect Compoment Index,
// 1 - position, 2 - normal, 3 - textureCoord, 4 - tangentU
static int lua_addVertex(lua_State * L);

// add a index to the indices, a index must be the 'index' of the vertices,
static int lua_addIndex(lua_State * L);

// show some help message.
static int lua_help(lua_State * L);

// called when the collect garbage.
static int lua_gc(lua_State * L);

// module lib, only a 'new' function.
static const struct luaL_Reg MeshDataLib[] =
{
	{"new", lua_newMeshData},
	{NULL, NULL}
};

// methods of the userData
static const struct luaL_Reg MeshDataLib_m[] =
{
	{ "show",				lua_showMeshData},
	{ "getDetail",			lua_getDetail },
	{ "addPosition",		lua_addPosition },
	{ "addNormal",			lua_addNormal },
	{ "addTangentU",		lua_addTangentU },
	{ "addTextureCoord",	lua_addTexC },
	{ "addVertex",			lua_addVertex},
	{ "addIndex",			lua_addIndex },
	{ "help",				lua_help },
	{ "__gc",				lua_gc },
	{NULL, NULL}
};
