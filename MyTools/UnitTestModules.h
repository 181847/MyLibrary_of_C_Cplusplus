#pragma once
#include <functional>
#include <string>
#include <vector>
#include <exception>
#include "MyAssert.h"

//---------------------------------------------------------------------------------
// A simple example to use this modules.
/*---------------------------------------------------------------------------------

#include <MyTools\RandomTool.h>
#include <MyTools\UnitTestModules.h>

DECLARE_TEST_UNITS;

namespace TestUnit
{
void GetReady() {}
void AfterTest() {}

void AddTestUnit()
{
#pragma region Try the testUnit
	TEST_UNIT_START("a test always success")
		return true;
	TEST_UNIT_END;
#pragma endregion

}// AddTestUnit()

}// namespace TestUnit

int main()
{
	TestUnit::testMain();
	return 0;
}

*///------------------------------------------------------------


#define EQ(a, b)	((a) == (b)) 
#define GT(a, b)	((a) >  (b))
#define LE(a, b)	((a) >  (b))
#define GT_EQ(a, b) ((a) >  (b))
#define LE_EQ(a, b) ((a) >  (b))
#define FT_EQ(fa, fb) (1e-5 > std::abs((fa) - (fb)))
#define NOT_EQ(a, b) ((a) != (b))

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

#define TEST_UNIT_END \
		return errorLogger.conclusion(); \
	}))

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
		std::printf("\t%s\t\t\t%s\n", result ? "success" : "failed", unitName.c_str());
	}
}

inline void Summary()
{
	gTestUnitCount = testUnits.size();
	std::printf("Summarize:\n");
	std::printf("TestCount\tsuccess\t\tfailed\n");
	std::printf("%d\t\t%d\t\t%d\n", gTestUnitCount, gSuccessCount, gTestUnitCount - gSuccessCount);
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
	catch (std::exception & e)
	{
		std::fprintf(stderr, "ERROR:%s", e.what());
	}

	std::getchar();
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
	inline void tick()
	{
		++_errorCount;
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
	inline void addErrorCount(size_t newErrorCount)
	{
		_errorCount += newErrorCount;
	}

	// add specific error count.
	inline ErrorLogger& operator += (size_t newErrorCount)
	{
		_errorCount += newErrorCount;
		return *this;
	}

	inline void LogError(size_t errorCount)
	{
		_errorCount += errorCount;
	}

	// log one error if not equal
	template<typename T1, typename T2>
	inline void LogIfNotEq(const T1& t1, const T2& t2)
	{
		addErrorCount(t1 != t2);
	}

#pragma region specialization
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for float
	template<>
	inline void LogIfNotEq<float, float>(const float& t1, const float& t2)
	{
		if (fabs(t1 - t2) > 1e-8f)
		{
			// nearly not equal
			addErrorCount(true);
		}
	}
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for double
	template<>
	inline void LogIfNotEq<double, double>(const double& t1, const double& t2)
	{
		if (abs(t1 - t2) > 1e-8)
		{
			// nearly not equal
			addErrorCount(true);
		}
	}
#pragma endregion

	// log one error if equal
	template<typename T1, typename T2>
	inline void LogIfEq(const T1& t1, const T2& t2)
	{
		addErrorCount(t1 == t2);
	}

#pragma region specialization
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for float
	template<>
	inline void LogIfEq<float, float>(const float& t1, const float& t2)
	{
		if (fabs(t1 - t2) < 1e-8f)
		{
			// nearly equal
			addErrorCount(true);
		}
	}
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for double
	template<>
	inline void LogIfEq<double, double>(const double& t1, const double& t2)
	{
		if (abs(t1 - t2) < 1e-8)
		{
			// nearly equal
			addErrorCount(true);
		}
	}
#pragma endregion

	// log one error if the argument is false
	inline void LogIfFalse(bool falseMeansOneError)
	{
		if (false == falseMeansOneError)
		{
			++_errorCount;
		}
	}

	// log one error if the argument is true
	inline void LogIfTrue(bool trueMeansOneError)
	{
		if (true == trueMeansOneError)
		{
			++_errorCount;
		}
	}
};

}// namespace TestUnit
