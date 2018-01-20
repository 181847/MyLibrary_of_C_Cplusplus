#pragma once
#include <vector>
#include "MathTool.h"
// this head file declare some useful function
// to generate the random numbers

// Declarations 
namespace RandomTool
{

// for general randome numbers, the NumberType must be an Interger Type, char, bool, int, wchar, etc.
template<typename NumberType = size_t>
class RandomSet
	:public std::vector<NumberType>
{
	static_assert(std::is_integral<NumberType>::value, "The NumberType must be an Integral, such as char, bool, int...");

public:
	// set seed
	void setSeed(int newSeed);
	// add one to the seed
	void tickSeed();

	// Generate random number range from min to max;
	void RandomNumbers(size_t length, NumberType min, NumberType max);

	// Generate random permutation range from 0 to length - 1 
	void RandomSequence(size_t length, int offset = 0);

private:
	// the seed used to rand
	int m_seed;
};

// the function definations
namespace Func 
{

// Try to shuffle the elements inside the vector.
template<typename NumberType>
void Shuffle(std::vector<NumberType>& toBeShuffled, int seed = 0);

}// namespace Func

}// namespace RandomTools


// Definations
namespace RandomTool
{

#pragma region RandomSet definations

template<typename NumberType>
void RandomSet<NumberType>::setSeed(int newSeed)
{
	m_seed = newSeed;
}

template<typename NumberType>
void RandomSet<NumberType>::tickSeed()
{
	++m_seed;
}

template<typename NumberType>
void RandomSet<NumberType>::RandomNumbers(size_t length, NumberType min, NumberType max)
{
	NumberType range = max - min;
	srand(m_seed);

	this->clear();
	this->resize(length);
	for (size_t i = 0; i < length; ++i)
	{
		this->operator[](i) = rand() % range + min;
	}
}

template<typename NumberType>
void RandomSet<NumberType>::RandomSequence(size_t length, int offset)
{
	srand(m_seed);
	this->clear();
	this->resize(length);
	for (NumberType i = 0; i < length; ++i)
	{
		this->operator[](i) = i;
	}

	// Switch random elements.
	for (size_t i = 0; i < length; ++i)
	{
		size_t randIndex = rand() % length;

		std::swap(this->operator[](i), this->operator[](randIndex));
	}

	// If no offset is setted, just return to avoid more calculations.
	if (offset == 0)
	{
		return;
	}

	for (auto & randomNumber : *this)
	{
		randomNumber += offset;
	}
}

#pragma endregion RandomSet definations


namespace Func
{

#pragma region Function definations

template<typename NumberType>
void Shuffle(std::vector<NumberType>& toBeShuffled, int seed)
{
	srand(seed);
	
	int length = toBeShuffled.size();

	assert(length > 0 && "vector should not be empty.");

	// Switch random elements.
	for (int i = 0; i < length; ++i)
	{
		int randIndex = rand() % length;

		std::swap(toBeShuffled[i], toBeShuffled[randIndex]);
	}
}

#pragma endregion

}// namespace Func

}// namespace RandomTool


