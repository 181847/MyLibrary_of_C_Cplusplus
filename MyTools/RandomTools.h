#pragma once
#include <vector>
// this head file declare some useful function
// to generate the random numbers

namespace randomTool
{


template<typename T>
inline void RandomSequence(size_t length, std::vector<T> * psequence, unsigned int seed = 1)
{
	auto & sequence = *psequence;
	srand(seed);
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


}// namespace randomTools

