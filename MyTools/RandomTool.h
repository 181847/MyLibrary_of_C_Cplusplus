#pragma once
#include <vector>
#include <array>
#include "MathTool.h"
#include "TypeTool.h"
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

// anynomous namespace for some useful tool
namespace
{

// isOfVector is used inside the Dispatch() function,
// to static_assert the Dispatch can only be used with std::vector.
template<typename T>
struct isOfVector { enum { value = false }; };
//||||||||||||||||||||||||||||||||||||||||||
template<typename T>
struct isOfVector<std::vector<T>>
{
	enum { value = true };
};

}
// Dispatch the Number to other vector.
// The vector must have the size,
// the origianl size must equal to the sum of the branches' size.
template<typename ARRAY, typename ...OTHER_ARRAY_LIST>
void Dispatch(ARRAY& original, OTHER_ARRAY_LIST& ...branches);

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

template<typename ARRAY, typename ...OTHER_ARRAY_LIST>
void Dispatch(ARRAY& original, OTHER_ARRAY_LIST& ...branches)
{
	static_assert(TypeTool::IsAllOf<TypeTool::TypeContainer<OTHER_ARRAY_LIST...>, ARRAY>::value,
		"Other OTHER_ARRAY_LIST type do not match with orignal");

	static_assert(isOfVector<ARRAY>::value, "ARRAY must be as vector.");

	static_assert(sizeof...(branches) > 0, "branches not enough");

	const int numBranches = sizeof...(branches);

	// Get the step size for each branch.
	int branchCount = 0;
	std::array<int, numBranches> stepList =
	{
		(branchCount += branches.size(), branchCount)...
	};

	assert(stepList[numBranches - 1] == original.size() &&
		"Total branches capacity not match with original.");

	// Helper function to push numbers in the orignal to oneBranch from 'from' to 'to'.
	auto PushNumberFunc = [&original](ARRAY& oneBranch, int from, int to) -> int
	{
		assert(from < to && "internal error");
		oneBranch.clear();
		for (int index = from; index < to; ++index)
		{
			oneBranch.push_back(original[index]);
		}
		return 0;
	};

	// Foreach step, push the numbers to specific branch.
	int phase = 0;
	int index = 0;
	// unpack
	stepList = 
	{
		(
			PushNumberFunc(branches, index, stepList[phase]),	// push number from original to each branch.
			index = stepList[phase],							// update index to the next position.
			++phase												// move to next phase.
		)...	// unpack the operation of branches.
	};
}

#pragma endregion

}// namespace Func

}// namespace RandomTool


