
file(GLOB_RECURSE PRE_LUA_SOURCE_FILES Lua-5.3.4/src/*.c)

set(LUA_SOURCE_FILES)

# remove lua.c and luac.c, because this is a dynamic link library.
FOREACH(item ${PRE_LUA_SOURCE_FILES})
	STRING(REGEX MATCH "^.*lua\\.c$" luaCItem ${item})
	STRING(REGEX MATCH "^.*luac\\.c$" luaCCItem ${item})
	IF(NOT luaCItem AND NOT luaCCItem)
		LIST(APPEND LUA_SOURCE_FILES ${item})
	ENDIF(NOT luaCItem AND NOT luaCCItem)
ENDFOREACH(item ${PRE_LUA_SOURCE_FILES})

message(STATUS "lua source files is:" "${LUA_SOURCE_FILES}")
	
add_library(LuaLib SHARED ${LUA_SOURCE_FILES})
target_compile_definitions(LuaLib PRIVATE LUA_BUILD_AS_DLL)
set_target_properties(LuaLib PROPERTIES OUTPUT_NAME "Lua")

set_target_properties(LuaLib
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Lua/Lua-5.3.4/dll"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Lua/Lua-5.3.4/dll"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Lua/Lua-5.3.4/dll")
