#include "LuaInterpreter.h"

namespace Lua
{

// each command containt a '\n' at the tail of the command.
const char * LuaInterpreter::exitInteractiveModeCommand = "exit()\n";

LuaInterpreter::LuaInterpreter()
{
	this->isMainThread = true;
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
}

LuaInterpreter::LuaInterpreter(lua_State * L, bool isMainThread)
{
	m_L = L;
	this->isMainThread = isMainThread;
}


LuaInterpreter::~LuaInterpreter()
{
	// only the mainThread can close
	if (isMainThread)
	{
		lua_close(m_L);
	}
}

LuaInterpreter * LuaInterpreter::Run()
{
	size_t commandLength = 0;
	fprintf(stdout, ">>");
	while (Not(stop) && fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		// retrive last '\n'
		if (0 == strcmp(buffer, exitInteractiveModeCommand))
		{
			fprintf(stdout, "you have entered 'exit()', interactive mode exited.\n");
			break;
		}
		error = luaL_loadstring(m_L, buffer) || lua_pcall(m_L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "error:\t%s\n", lua_tostring(m_L, -1));
			lua_pop(m_L, 1);
		}
		fprintf(stdout, ">>");
	}
	return this;
}

int LuaInterpreter::GetStackSize()
{
	return lua_gettop(m_L);
}

PLuaInterpreter LuaInterpreter::GetGlobal(const char * varname)
{
	int type = lua_getglobal(m_L, varname);
	ASSERT(type != LUA_TNONE);
	return this;
}

PLuaInterpreter LuaInterpreter::SetGlobalAndPop(const char * varname)
{
	lua_setglobal(m_L, varname);
	return this;
}

PLuaInterpreter LuaInterpreter::GetFieldOnTop(const char * key)
{
	lua_getfield(m_L, -1, key);
	return this;
}

PLuaInterpreter LuaInterpreter::GetIndexOnTop(const lua_Integer index)
{
	lua_geti(m_L, -1, index);
	return this;
}

PLuaInterpreter LuaInterpreter::Pop()
{
	lua_pop(m_L, 1);
	return this;
}

PLuaInterpreter LuaInterpreter::DoFile(const char * file)
{
	int error = luaL_loadfile(m_L, file) || lua_pcall(m_L, 0, 0, 0);
	if (error)
	{
		fprintf(stderr, "error:\t%s\n", lua_tostring(m_L, -1));
		// must throw the exception.
		ThrowIfFalse(false && "do file Error");
	}
	return this;
}

bool LuaInterpreter::IsNil()
{
	return lua_isnil(m_L, -1);
}

PLuaInterpreter LuaInterpreter::IsNil(bool * isNil)
{
	ASSERT(isNil);
	*isNil = lua_isnil(m_L, -1);
	return this;
}

PLuaInterpreter LuaInterpreter::If(
	std::function<bool(PLuaInterpreter)> condition, 
	std::function<void(PLuaInterpreter)> Then, 
	std::function<void(PLuaInterpreter)> Else)
{
	//ThrowIfFalse(false && "this function is no completed, don't use it!");
	if (condition(this))
		Then(this);
	else
		Else(this);
	return this;
}

PLuaInterpreter LuaInterpreter::Foreach(
	std::function<		void(LUA_INTERPRETER_FOREACH_LAMBDA_ARGS)> work)
{
	ASSERT(LUA_TTABLE == lua_type(m_L, -1));
	int				keyType		= LUA_TNONE;
	UINT			keyItg		= -1;
	const char *	keyStr		= nullptr;
	// the nextIterationSize will record the size of next iteration of the field,
	// because each iteration need the key at the top,
	// and the value must be popped before next iteration,
	// for simplification , here we just resume the stack statues
	// as before the 'work' called, through poping the extra element
	// the 'work' added, if the 'work' pop the element to where 
	// below the original key, we throw a SimpleException.
	int	nextIterationSize = 0;
	int shouldBePopedSize = 0;

	lua_pushnil(m_L);
	// record the original key's location.
	nextIterationSize = lua_gettop(m_L);
	// remaind that before each iteration, key is on the top,
	// after calling 'lua_next', the value is on the top,
	// for next iteration, we must 're'pop the value,
	// and keep the key on the top before calling 'lua_next'.
	while (lua_next(m_L, -2))
	{

		// get key type
		keyType = lua_type(m_L, -2);
		ASSERT(keyType == LUA_TNUMBER || keyType == LUA_TSTRING);

		switch (keyType)
		{
		case LUA_TNUMBER:
			keyItg = static_cast<UINT>(lua_tointeger(m_L, -2));
			keyStr = nullptr;
			break;


		case LUA_TSTRING:
			keyItg = -1;
			keyStr = lua_tostring(m_L, -2);
			break;
		}
		// let the outer function do with the value which sit on
		// the top of the stack.
		work(this, keyType == LUA_TNUMBER, keyItg, keyStr);

		// resume the stack for next iteration,
		// the nextIterationSize is the size of the stack
		// when after putting a nil to the top 
		// and before the lua_state enter the lua_next(...).
		shouldBePopedSize = lua_gettop(m_L) - nextIterationSize;
		ASSERT(shouldBePopedSize >= 0);
		if (shouldBePopedSize)
			lua_pop(m_L, shouldBePopedSize);
	}// while

	// be care that the last lua_next() return 0 
	// and it automatically pop the key,
	// to keep the stack clear.
	return this;
}

PLuaInterpreter LuaInterpreter::Call(int functionIndex, UINT paramCount, UINT returnCount)
{
	int error = lua_pcall(m_L, paramCount, returnCount, 0);
	ThrowIfFalse(error == LUA_OK);
	return this;
}

PLuaInterpreter LuaInterpreter::PushInteger(lua_Integer itg)
{
	lua_pushinteger(m_L, itg);
	return this;
}

PLuaInterpreter LuaInterpreter::PushNumber(lua_Number num)
{
	lua_pushnumber(m_L, num);
	return this;
}

PLuaInterpreter LuaInterpreter::PushString(const char * str)
{
	lua_pushstring(m_L, str);
	return this;
}

PLuaInterpreter LuaInterpreter::ConstainStackSizeMax(std::function<void(PLuaInterpreter)> opt)
{
	int size = lua_gettop(m_L);
	opt(this);
	size = lua_gettop(m_L) - size;
	
	if (size > 0)
	{
		lua_pop(m_L, size);
	}
	
	return this;
}


}// namespace Lua
