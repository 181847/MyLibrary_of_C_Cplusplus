#pragma once

#include "../../MyTools/LuaTools.h"
#include "../../MyTools/UsefulDataType.h"

#define Not(x) (!x)

#pragma comment (lib, "Lua.lib")

#ifdef LUAINTERPRETER_EXPORTS
#define LUAINTERPRETER_API __declspec(dllexport)
#else
#ifdef LUAINTERPRETER_IMPORTS
#define LUAINTERPRETER_API __declspec(dllimport)
#endif
#endif

// here is the marco to short the function prameter presentation
// in the LuaInterpreter::Foreach
#define  LUA_INTERPRETER_FOREACH_LAMBDA_ARGS\
	/* lambda parameter->*/		Lua::PLuaInterpreter	luaInterForeach,	bool			keyIsNumber,\
	/* lambda parameter->*/		UINT					keyItg,				const char *	keyStr

// next three marco is used in Lua::LuaInterpreter::Foreach(...)
// to start and end a lambda to to be the parameter of the 'Lua::LuaInte...Foreach(...)',
// it start a lambda that automaticlly capture 
// the references of the variable.
#define LUA_INTERPRETER_FOREACH_LAMBDA_START\
	[&](LUA_INTERPRETER_FOREACH_LAMBDA_ARGS){
#define EACH luaInterForeach
#define LUA_INTERPRETER_FOREACH_LAMBDA_END }

// next three marco used in Lua::LuaInterpreter::ConstainStackSizeMax(...)
// to start and end a lambda to be the parameter of the 'Lua::...Constra..Max(...)',
// it automatically capture the references of the variables.
#define CONSTRAIN_START [&](Lua::PLuaInterpreter pLuaInterConstrain) -> void{
#define CONSTRAIN_TARGET pLuaInterConstrain
#define CONSTRAIN_END }

namespace Lua
{

class LUAINTERPRETER_API LuaInterpreter;
typedef LuaInterpreter * PLuaInterpreter;

class LUAINTERPRETER_API LuaInterpreter
{
	const static char * exitInteractiveModeCommand;
public:
	LuaInterpreter();
	LuaInterpreter(lua_State * L, bool isMainThread = false);
	~LuaInterpreter();

	DELETE_COPY_CONSTRUCTOR(LuaInterpreter)

	// start running 
	PLuaInterpreter Run();

	int GetStackSize();

	// get the top element as a number,
	// throw SimpleException if it is not a number,
	// only if there is no error,
	// the number will be pop from the stack.
	template<typename NUMBER_TYPE = lua_Number>
	NUMBER_TYPE ToNumberAndPop();
	//|||||||||||||||||||||||||||||||||||||
	template<typename NUMBER_TYPE = lua_Number>
	PLuaInterpreter ToNumberAndPop(NUMBER_TYPE * outNumber);

	// get the top element as integer,
	// throw SimpleException if it is not a number,
	// only if there is no error,
	// the integer will be pop from the stack.
	template<typename INTEGER_TYPE = lua_Integer>
	INTEGER_TYPE ToIntegerAndPop();
	//|||||||||||||||||||||||||||||||||||||
	template<typename INTEGER_TYPE = lua_Integer>
	PLuaInterpreter ToIntegerAndPop(INTEGER_TYPE * outInteger);

	// get the top element as string,
	// throw SimpleException if it is not a String or 
	// the BufferSize is less than the string length - 1,
	// copy the string into the buffer,
	// then pop the string from the stack.
	template<int BufferSize>
	PLuaInterpreter ToStringAndClear(char * buffer);

	// push a global value to the top of the stack
	PLuaInterpreter GetGlobal(const char * varname);

	// set the top element as a global variable with the name
	PLuaInterpreter SetGlobalAndPop(const char * varname);

	// getField on the top element.
	PLuaInterpreter GetFieldOnTop(const char * key);

	// get indexed field of the top 
	PLuaInterpreter GetIndexOnTop(const lua_Integer index);

	// pop one element on the top of the stack
	PLuaInterpreter Pop();

	// load and call the file,
	// throws SimpleException whenever there is an error.
	PLuaInterpreter DoFile(const char * file);

	// can use lambda to directly control the lua_State;
	PLuaInterpreter Do(std::function<void(lua_State * L)> func);

	// Is the top element is nil?
	bool IsNil();

	// to ensure the continuous of the operation of LuaInterpreter,
	// add a branch to the function,
	// the condition function to return true or false,
	// if it return true, the 'Then' function will be called,
	// if false, the 'Else' function instead.
	PLuaInterpreter If(
		std::function<bool(PLuaInterpreter)> condition,
		std::function<void(PLuaInterpreter)> Then, 
		std::function<void(PLuaInterpreter)> Else);

	// use a outer function to convert the userData(void*)
	// to the specific pointer type.
	template<typename USERDATA_TYPE>
	USERDATA_TYPE *
	ToUserDataAndClear(
		const char * metaTable, 
		std::function<USERDATA_TYPE*(USERDATA_TYPE*)> converter =
			[](USERDATA_TYPE * pointer) {return pointer; });
	//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// be careful that we must pass in a pointer which point to a pointer,
	// but the lambda paramter and return value is just a pointer to userdata.
	template<typename USERDATA_TYPE>
	PLuaInterpreter
		ToUserDataAndClear(
			const char * metaTable,
			USERDATA_TYPE ** outUserdata,
			std::function<USERDATA_TYPE*(USERDATA_TYPE*)> converter 
				= [](USERDATA_TYPE * pointer) {return pointer; });

	// ensure the table sit on the top of the stack,
	// on each iteration, the user should pop the value.
	PLuaInterpreter Foreach(
		std::function<void(LUA_INTERPRETER_FOREACH_LAMBDA_ARGS)> work);

	// call a lua function, 
	// If error happend throws a SimpleException.
	PLuaInterpreter Call(int functionIndex, UINT paramCount, UINT returnCount);

	 // push data to the interpreter
	PLuaInterpreter PushInteger(lua_Integer itg);
	// ||||||||||||||||||||||||||||||||||||||||||||
	PLuaInterpreter PushNumber(lua_Number num);
	// |||||||||||||||||||||||||||||||||||||||||||| 
	PLuaInterpreter PushString(const char * str);

	// get the length of the top element.
	template<typename INTEGER_TYPE = lua_Integer>
	INTEGER_TYPE LengthOfTop();
	// |||||||||||||||||||||||||||||||||||||||||||| 
	template<typename INTEGER_TYPE = lua_Integer>
	PLuaInterpreter LengthOfTop(INTEGER_TYPE * pItg);

	// this function ensure that after the opt called,
	// the stack size dosen't grow up (but it is possible to drop down),
	// it actually just pop the element of the overflow the previous stack size.
	PLuaInterpreter ConstainStackSizeMax(std::function<void(PLuaInterpreter)> opt);

public:
	bool stop = false;
	bool isMainThread = false;
	lua_State * m_L;

private:
	// use to store the command line.
	char buffer[255];
	int error;
};

template<typename NUMBER_TYPE>
inline NUMBER_TYPE LuaInterpreter::ToNumberAndPop()
{
	ASSERT(m_L);
	int isNum = 0;
	NUMBER_TYPE ret = static_cast<NUMBER_TYPE>(lua_tonumberx(m_L, -1, &isNum));
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return static_cast<NUMBER_TYPE>(ret);
}

template<typename NUMBER_TYPE>
inline PLuaInterpreter LuaInterpreter::ToNumberAndPop(NUMBER_TYPE * outNumber)
{
	ASSERT(m_L);
	int isNum = 0;
	*outNumber = static_cast<NUMBER_TYPE>(lua_tonumberx(m_L, -1, &isNum));
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return this;
}

template<typename INTEGER_TYPE>
inline INTEGER_TYPE LuaInterpreter::ToIntegerAndPop()
{
	ASSERT(m_L);
	int isNum = 0;
	INTEGER_TYPE ret = static_cast<INTEGER_TYPE>(lua_tointegerx(m_L, -1, &isNum));
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return ret;
}

template<typename INTEGER_TYPE>
inline PLuaInterpreter LuaInterpreter::ToIntegerAndPop(INTEGER_TYPE * outInteger)
{
	ASSERT(m_L);
	int isNum = 0;
	*outInteger = static_cast<INTEGER_TYPE>(lua_tointegerx(m_L, -1, &isNum));
	ThrowIfFalse(isNum);
	lua_pop(m_L, 1);
	return this;
}

template<int BufferSize>
inline PLuaInterpreter LuaInterpreter::ToStringAndClear(char * buffer)
{
	ASSERT(m_L);
	size_t stringLen;
	const char * tempString = lua_tolstring(m_L, -1, &stringLen);

	// if tempString is nullptr, then throw exception
	ThrowIfFalse(tempString);
	ThrowIfFalse(BufferSize > stringLen + 1);

	strcpy_s(buffer, BufferSize, tempString);
	lua_pop(m_L, 1);
	return this;
}

template<typename USERDATA_TYPE>
inline USERDATA_TYPE *
LuaInterpreter::ToUserDataAndClear(
	const char * metaTableName, 
	std::function<USERDATA_TYPE*(USERDATA_TYPE*)> converter)
{
	auto * pointer = reinterpret_cast<USERDATA_TYPE*>(
		luaL_checkudata(m_L, -1, metaTableName));
	ThrowIfFalse(pointer);
	Pop();
	// pop the userData
	return converter(pointer);
}

template<typename USERDATA_TYPE>
inline PLuaInterpreter LuaInterpreter::ToUserDataAndClear(
	const char * metaTable, 
	USERDATA_TYPE ** outUserdata, 
	std::function<USERDATA_TYPE*(USERDATA_TYPE*)> converter)
{
	*outUserdata = reinterpret_cast<USERDATA_TYPE*>(
		luaL_checkudata(m_L, -1, metaTable));
	ThrowIfFalse(*outUserdata);
	*outUserdata = converter(*outUserdata);
	return this;
}

template<typename INTEGER_TYPE>
inline INTEGER_TYPE LuaInterpreter::LengthOfTop()
{
	return static_cast<INTEGER_TYPE>(luaL_len(m_L, -1));
}

template<typename INTEGER_TYPE>
inline PLuaInterpreter LuaInterpreter::LengthOfTop(INTEGER_TYPE * pItg)
{
	*pItg = static_cast<INTEGER_TYPE>(luaL_len(m_L, -1));
	return this;
}


}// namespace Lua
