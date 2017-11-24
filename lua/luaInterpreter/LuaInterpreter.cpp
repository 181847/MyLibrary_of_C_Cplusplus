#include "LuaInterpreter.h"

LuaInterpreter::LuaInterpreter()
{
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
}


LuaInterpreter::~LuaInterpreter()
{
	lua_close(m_L);
}

void LuaInterpreter::Run()
{
	fprintf(stdout, ">>");
	while (Not(stop) && fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		error = luaL_loadstring(m_L, buffer) || lua_pcall(m_L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "error:\t%s\n", lua_tostring(m_L, -1));
			lua_pop(m_L, 1);
		}
		fprintf(stdout, ">>");
	}
	
	stop = true;
}

int LuaInterpreter::GetStackSize()
{
	return lua_gettop(m_L);
}

lua_Number LuaInterpreter::ToNumberAndClear()
{
	ASSERT(m_L);
	int isNum = 0;
	auto ret = lua_tonumberx(m_L, -1, &isNum);
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return ret;
}

lua_Integer LuaInterpreter::ToIntegerAndClear()
{
	ASSERT(m_L);
	int isNum = 0;
	auto ret = lua_tointegerx(m_L, -1, &isNum);
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return ret;
}

void LuaInterpreter::GetFieldOnTop(const char * key)
{
	lua_getfield(m_L, -1, key);
}

void LuaInterpreter::GetIndexOnTop(const lua_Integer index)
{
	lua_geti(m_L, -1, index);
}

void LuaInterpreter::Pop()
{
	lua_pop(m_L, 1);
}

void LuaInterpreter::Do(std::function<void(lua_State*L)> func)
{
	func(m_L);
}

bool LuaInterpreter::IsNil()
{
	return lua_isnil(m_L, -1);
}
