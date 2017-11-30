#pragma once
#include <functional>
#include <string>
#include <vector>
#include "MyAssert.h"

/*
** to use this module , please use DECLARE_TEST_UNITS; at the head
** of the executed .cpp file, 
** implements the 'void TestUnit::GetReady()'/'void TestUnit::AfterTest();'
** implements the 'void TestUnit::AddTestUnit()', you can find more at
** the comment of the 'void AddTestUnit()'.
*/

extern std::vector<std::function<bool(std::string& unitName)>> testUnits;

extern unsigned int totalCount;
extern unsigned int	success;

#define DECLARE_TEST_UNITS\
	static std::vector<std::function<bool(std::string& unitName)>>\
	testUnits;\
	static unsigned int totalCount	= 0;\
	static unsigned int	success		= 0;

#define TEST_UNIT_START(UnitName) testUnits.push_back(std::move(\
		[](std::string& unitName) -> bool {\
			unitName = UnitName;

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
// whether the unit test success or not.
// For example:
//		TEST_UNIT_START("a test will always success")
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
		success += result;
		printf("\t%s\t\t\t%s\n", result ? "success" : "failed", unitName.c_str());
	}
}

inline void Summary()
{
	totalCount = testUnits.size();
	printf("Summarize:\n");
	printf("TestCount\tsuccess\t\tfailed\n");
	printf("%d\t\t%d\t\t%d\n", totalCount, success, totalCount - success);
}

inline void execute()
{
	GetReady();
	AddTestUnit();
	RunTest();
	Summary();
	AfterTest();
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

}// namespace TestUnit
