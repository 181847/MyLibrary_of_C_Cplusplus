#pragma once
#include <functional>
#include <string>
#include <vector>
#include "MyAssert.h"

#define EQ(a, b)	(a == b) 
#define GT(a, b)	(a >  b)
#define LE(a, b)	(a >  b)
#define GT_EQ(a, b) (a >  b)
#define LE_EQ(a, b) (a >  b)
#define FT_EQ(fa, fb) (1e-5 > std::abs(fa - fb))
#define NOT_EQ(a, b) (a != b)

#define NO_ERROR(errorCount) EQ(0, errorCount)

#define CHECK_ARR_2(arr, COMPARE, e0, e1)\
	(COMPARE(arr[0], e0) && COMPARE(arr[1], e1))

#define CHECK_ARR_3(arr, COMPARE, e0, e1, e2) \
	(COMPARE(arr[0], e0) && COMPARE(arr[1], e1) && COMPARE(arr[2], e3)

/*
** to use this module , please use DECLARE_TEST_UNITS; at the head
** of the executed .cpp file, 
** implements the 'void TestUnit::GetReady()'/'void TestUnit::AfterTest();'
** implements the 'void TestUnit::AddTestUnit()', you can find more at
** the comment of the 'void AddTestUnit()'.
*/

extern std::vector<std::function<bool(std::string& unitName)>> testUnits;

extern unsigned int gTestUnitCount;
extern unsigned int	gSuccessCount;


// the marco start declare the units,
// inside, the marco have declare an errorLogger,
// which is used to log the error count inside the testUnit,
// the user can use it to log the error count,
// and return the errorLogger.conclusion(),
// it will return true, if no error happened.
#define DECLARE_TEST_UNITS											\
	static std::vector<std::function<bool(std::string& unitName)>>	\
	testUnits;														\
	static unsigned int gTestUnitCount	= 0;						\
	static unsigned int	gSuccessCount	= 0;

#define TEST_UNIT_START(UnitName)			\
	testUnits.push_back(std::move(			\
		[](std::string& unitName) -> bool {	\
			unitName = UnitName;			\
	TestUnit::ErrorLogger errorLogger;		// for each testUnit have a errorLogger to help log the errorCount.

#define TEST_UNIT_END }))

namespace TestUnit 
{

// the user should implements this function 
// to get some previous work for test.
// such as initialize the tested variable.
// there is no way to pass arguments between the functions,
// so just use the global variable.
void GetReady();
void AfterTest();

// user should implemment this function
// to add the test units,
// for simple using , just 
// use the macro 'TEST_UNIT_START(UnitName)' and 
// 'TEST_UNIT_END' to include the desired code.
// Inside the two marco , there is a lambda,
// the only thing that you will concen about is the return 
// of the lambda.
// The return value should be a boolean, which indicate
// whether the unit test gSuccessCount or not.
// For example:
//		TEST_UNIT_START("a test will always gSuccessCount")
//			...do the job...
//			return true
//		TEST_UNIT_END
void AddTestUnit();
inline void RunTest()
{
	std::string unitName;
	for (auto & testUnit : testUnits)
	{
		bool result = testUnit(unitName);
		gSuccessCount += result;
		printf("\t%s\t\t\t%s\n", result ? "gSuccessCount" : "failed", unitName.c_str());
	}
}

inline void Summary()
{
	gTestUnitCount = testUnits.size();
	printf("Summarize:\n");
	printf("TestCount\tgSuccessCount\t\tfailed\n");
	printf("%d\t\t%d\t\t%d\n", gTestUnitCount, gSuccessCount, gTestUnitCount - gSuccessCount);
}

inline void execute()
{
	GetReady();
	AddTestUnit();
	RunTest();
	AfterTest();
	Summary();
}

inline void testMain()
{
	try
	{
		execute();
	}
	catch (SimpleException & e)
	{
		fprintf(stderr, "ERROR:%s", e.ToString().c_str());
	}

	getchar();
}

struct ErrorLogger
{
private:
	size_t _errorCount = 0;

public:
	ErrorLogger() : _errorCount(0) {}

	inline bool conclusion()
	{
		return _errorCount == 0;
	}

	inline size_t getErrorCount() const
	{
		return _errorCount;
	}

	// errorCount increase by one
	inline ErrorLogger& tick()
	{
		++_errorCount;
		return *this;
	}

	// errorCount increase by one
	inline ErrorLogger& operator ++ ()
	{
		++_errorCount;
		return *this;
	}

	// errorCount increase by one,
	// but return the logger after the increasment.
	// it is same as the ++errorLogger;
	inline ErrorLogger& operator ++ (int)
	{
		++_errorCount;
		return *this;
	}

	// add specific error count.
	inline ErrorLogger& addErrorCount(size_t newErrorCount)
	{
		_errorCount += newErrorCount;
		return *this;
	}

	// add specific error count.
	inline ErrorLogger& operator += (size_t newErrorCount)
	{
		_errorCount += newErrorCount;
		return *this;
	}

	// if pass in a pointer,
	// increase the _errorCount when the pointer is nullptr.
	/*template<typename PointerType>
	inline ErrorLogger& operator += (PointerType pointer)
	{
		_errorCount += (pointer == nullptr);
		return *this;
	}*/
};

}// namespace TestUnit
