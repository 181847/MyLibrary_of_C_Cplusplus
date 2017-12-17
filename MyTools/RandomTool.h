#pragma once
#include <vector>
#include "MathTool.h"
// this head file declare some useful function
// to generate the random numbers

namespace RandomTool
{

// for general randome numbers
template<typename NumberType>
class RandomSet
	:public std::vector<NumberType>
{
public:
	// set seed
	void setSeed(int newSeed);
	// add one to the seed
	void tickSeed();

	void randomNumbers(size_t length, NumberType min, NumberType max);
	void randomSequence(size_t length, int offset = 0);

private:
	// the seed used to rand
	int _seed;
};

template<typename NumberType>
void
RandomSet<NumberType>::
setSeed(int newSeed)
{
	_seed = newSeed;
}

template<typename NumberType>
void 
RandomSet<NumberType>::
tickSeed()
{
	++_seed;
}

template<typename NumberType>
void 
RandomSet<NumberType>::
randomNumbers(
	size_t length, NumberType min, NumberType max)
{
	NumberType range = max - min;
	srand(seed);

	this->clear();
	this->resize(length);
	for (size_t i = 0; i < length; ++i)
	{
		this->operator[](i) = rand() % range + min;
	}
}

template<typename NumberType>
void 
RandomSet<NumberType>::
randomSequence
(size_t length, int offset)
{
	srand(_seed);
	this->clear();
	this->resize(length);
	for (NumberType i = 0; i < length; ++i)
	{
		this->operator[](i) = i;
	}

	for (size_t i = 0; i < length; ++i)
	{
		size_t randIndex = rand() % length;

		std::swap(this->operator[](i), this->operator[](randIndex));
	}

	for (auto & randomNumber : *this)
	{
		randomNumber += offset;
	}
}


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

