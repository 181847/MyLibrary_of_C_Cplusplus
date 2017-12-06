#pragma once

// this head file define some useful function for math

namespace MathTool
{
template<typename T>
inline T clamp(const T& target, const T& min, const T& max)
{
	if (target < min)
	{
		return min;
	}
	else if (target > max)
	{
		return max;
	}
	else
	{
		return target;
	}
}

}// namespace MathTool
