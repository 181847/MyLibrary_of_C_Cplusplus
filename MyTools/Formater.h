#pragma once
#include <stdarg.h>
#include <stdio.h>
template<int BufferSize>
class Formater
{
public:
	Formater();
	~Formater();

	const char * __cdecl Formater<BufferSize>::format(const char * fmt, ...);
	const char * format(const char * string);
	const char * format(int i);

	bool operator ==(const char * compto);

	char * bufferPointer();
	enum {Size = BufferSize};

private:
	char  buffer[BufferSize];
};



template<int BufferSize>
Formater<BufferSize>::Formater()
{
}

template<int BufferSize>
Formater<BufferSize>::~Formater()
{
}

template<int BufferSize>
const char * Formater<BufferSize>::format(const char * string)
{
	sprintf_s(buffer, BufferSize, "%s", string);
	return buffer;
}

template<int BufferSize>
const char * Formater<BufferSize>::format(int i)
{
	sprintf_s(buffer, BufferSize, "%d", i);
	return buffer;
}

template<int BufferSize>
char * Formater<BufferSize>::bufferPointer()
{
	return buffer;
}

template<int BufferSize>
const char * __cdecl Formater<BufferSize>::format(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, BufferSize, fmt, args);
	va_end(args);
	return buffer;
}

template<int BufferSize>
bool Formater<BufferSize>::operator == (const char * compto)
{
	return 0 == strcmp(buffer, compto);
}
