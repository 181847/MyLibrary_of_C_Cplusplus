#pragma once
#include <vector>
#include "MathTool.h"
// this head file declare some useful function
// to generate the random numbers

namespace RandomTool
{

// this function to generate the random order of 0~(length-1)
template<typename T>
inline void RandomSequence(size_t length, std::vector<T> * psequence, unsigned int seed = 1)
{
	auto & sequence = *psequence;
	srand(seed);
	sequence.clear();
	sequence.resize(length);
	for (size_t i = 0; i < length; ++i)
	{
		sequence[i] = i;
	}
	
	for (size_t i = 0; i < length; ++i)
	{
		int randIndex = rand() % length;
		
		std::swap(sequence[i], sequence[randIndex]);
	}
}

template<typename T>
inline void RandomNumbers(
	size_t length,
	std::vector<T> * pcontainer,
	T max,
	T min = 0,
	unsigned int seed = 1)
{
	auto & container = *pcontainer;
	srand(seed);
	
	container.clear();
	container.resize(length);
	for (size_t i = 0; i < length; ++i)
	{
		container[i] = MathTool::clamp<T>(rand() % max, min, max);
	}
}


}// namespace randomTools

