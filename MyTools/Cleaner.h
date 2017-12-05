#pragma once
#include <functional>
class Cleaner
{
public:
	Cleaner(std::function<void(void)> cleanFunc)
		:cleanFunction(cleanFunc)
	{}
	~Cleaner()
	{
		cleanFunction();
	}

	Cleaner(const Cleaner&) = delete;
	Cleaner& operator = (const Cleaner&) = delete;

private:
	std::function<void(void)> cleanFunction;
};

