#pragma once
#include <functional>
#include <string>
#include <vector>
#include <exception>
#include <assert.h>
#include <chrono>
#include <iostream>

/********************************************.
|                                            |
.********************************************/


/*!
	\brief MACRO HELP TO USE THIS MODULE 
    start module declaration
*/
#define TEST_MODULE_START \
	namespace TestUnit\
	{\
		void AddTestUnit(std::vector<std::function<unsigned int(std::string& unitName, TimeCounter&)>>& appendToQuery)\
		{

/*!
	\brief MACRO HELP TO USE THIS MODULE 
    end module declaration
*/
#define TEST_MODULE_END \
		}\
	}\
	int main()\
	{\
		TestUnit::execute();\
		return 0;\
	}\

/*!
	\brief start a unit test.
*/
#define TEST_UNIT_START(UnitName)			\
	appendToQuery.push_back(std::move(			\
		[](std::string& unitName, TimeCounter& timeCounter) -> unsigned int {	\
			unitName = UnitName;			\
	TestUnit::ErrorLogger errorLogger;		// for each testUnit have a errorLogger to help log the errorCount.
    

/*!
	\brief stop a unit test.
*/
#define TEST_UNIT_END \
		return errorLogger.conclusion(); \
	}))

namespace TestUnit 
{


	/*!
		\brief which unit to measure time elapsed.
	*/
	using DurationType = std::chrono::microseconds;

	/*!
		\brief give the time unit a name to be printed in the output, e.g. "microseconds" or "seconds"...
	*/
	const std::string DURATION_TYPE_NAME = "ms";

    

/*!
	\brief a light struct to help record error count.
*/
struct ErrorLogger
{
private:
	size_t m_errorCount = 0;

public:
	ErrorLogger() : m_errorCount(0) {}

	inline int conclusion()
	{
		return m_errorCount;
	}

	inline size_t getErrorCount() const
	{
		return m_errorCount;
	}

	// errorCount increase by one
	inline void tick()
	{
		++m_errorCount;
	}

	// errorCount increase by one
	inline ErrorLogger& operator ++ ()
	{
		++m_errorCount;
		return *this;
	}

	// errorCount increase by one,
	// but return the logger after the increasment.
	// it is same as the ++errorLogger;
	inline ErrorLogger& operator ++ (int)
	{
		++m_errorCount;
		return *this;
	}

	// add specific error count.
	inline void addErrorCount(size_t newErrorCount)
	{
		m_errorCount += newErrorCount;
	}

	// add specific error count.
	inline ErrorLogger& operator += (size_t newErrorCount)
	{
		m_errorCount += newErrorCount;
		return *this;
	}

	inline void LogError(size_t errorCount)
	{
		m_errorCount += errorCount;
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
			++m_errorCount;
		}
	}

	// log one error if the argument is true
	inline void LogIfTrue(bool trueMeansOneError)
	{
		if (true == trueMeansOneError)
		{
			++m_errorCount;
		}
	}
};

/*!
    \brief TimeCounter used to record time for the test.
    each unit test will have one TimeCounter inside it,
    user can reuse it in the code to accumulate the expected time to be recored.
    For example, in one test, you may do some calculations, then write the result into the local file.
    What you concen about is how many time it costs to calculate the result,
    rather than writing result to the local file(writing will be slow if the result is large such as 2GB).
*/
class TimeCounter
{
public:
    /*!
        \brief the sum of the time that have been recorded.
    */
    DurationType m_sumDuration = DurationType::zero();
};

/*!
    \brief use RAII to record a period of time and add them to TimeCounter.
*/
struct TimeGuard
{
private:
    TimeCounter& m_tarteTimeCounter;
    std::chrono::system_clock::time_point m_startTimePoint;

public:
    TimeGuard(TimeCounter& addTo)
        :m_tarteTimeCounter(addTo)
    {
        m_startTimePoint = std::chrono::system_clock::now();
    }

    ~TimeGuard()
    {
        auto endTimePoint = std::chrono::system_clock::now();

        m_tarteTimeCounter.m_sumDuration += std::chrono::duration_cast<DurationType>(endTimePoint - m_startTimePoint);
    }
};

/*!
	\brief store brief test result such as test number and success number.
*/
struct TestResult
{
	unsigned int m_countTests = 0;
	unsigned int m_countSuccess = 0;
};

/*!
	\brief add test units into the array
	\param appendToQuery append the test units into this array
*/
void AddTestUnit(std::vector<std::function<unsigned int(std::string& unitName, TimeCounter&)>>& appendToQuery);

/*!
    \brief print a progress percentage into the screen(same line).
    \param progress a number between 0.0f and 1.0f which will be interpret to 0% to 100%.
    this is for some long term test to show some information to user.
*/
inline void ShowProgress(const float progress)
{
    std::printf("Progress: %.6f%%\r", progress * 100.0f);
}

/*!
	\brief run unit tests in the array'testUnits'
	\param testUnits test to be run
	\param result return brief result
*/
inline void RunTest(std::vector<std::function<unsigned int(std::string&, TimeCounter&)>>& testUnits, TestResult& result)
{
	std::string		unitName;
	unsigned int	errorCount;
	bool			isSuccess = false;

	for (auto & testUnit : testUnits)
	{
		errorCount = 0;
        TimeCounter insideCounter;
        TimeCounter unitTimeCounter;
		try 
		{
            TimeGuard guard(unitTimeCounter);
			errorCount = testUnit(unitName, insideCounter);
		}
		catch (std::exception e)
		{
			std::cout << "\t" << unitName << "exception happend: " << e.what() << std::endl;
			continue;
		}

		isSuccess = ( errorCount == 0 );
		if (isSuccess)
		{
			++result.m_countSuccess;
		}

		std::printf(
			// success/failed	errorCount	costTime   unitName
			"\t%s"              "\t%d"      "\t%8lld %s"   "\t%8lld %s"  "\t\t%s\n", 
			isSuccess ? "success" : "failed", 
			errorCount, 
            unitTimeCounter.m_sumDuration.count(), DURATION_TYPE_NAME.c_str(),
            insideCounter.m_sumDuration.count(), DURATION_TYPE_NAME.c_str(),
			unitName.c_str());
	}
}

/*!
	\brief show result in the console
	\param result the brief result returned by RunTest.
*/
inline void Summary(const TestResult& result)
{
	std::printf("Summarize:\n");
	std::printf("TestCount\tsuccess\t\tfailed\n");
	std::printf("%d\t\t%d\t\t%d\n", result.m_countTests, result.m_countSuccess, result.m_countTests - result.m_countSuccess);
}

/*!
	\brief core function in which to add user-defined tests -> run them -> show summary
	may be we can 
*/
inline void execute()
{
	std::vector<std::function<unsigned int(std::string& unitName, TimeCounter&)>> testUnitQuery;
	AddTestUnit(testUnitQuery);

	TestResult result;
	result.m_countTests = testUnitQuery.size();
	RunTest(testUnitQuery, result);

	Summary(result);

	// wait for user enter.
	std::getchar();
}

}// namespace TestUnit
