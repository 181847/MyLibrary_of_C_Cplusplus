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

#ifdef WIN32
/*!
    \brief assert on the expr, increase errorCount if failed.
    and put a break point if testParameter.m_breakIfTestAssertionFailed is true.
    user can only use this marco inside the TEST_UNIT.
*/
#define TEST_ASSERT(expr) do {if ( ! (expr) ) {\
            if (testParameter.m_breakIfTestAssertionFailed)\
            {\
                __debugbreak();\
            }; \
            errorLogger.LogError();\
        }\
    } while(0)
#endif

/*!
	\brief MACRO HELP TO USE THIS MODULE 
    start module declaration
*/
#define TEST_MODULE_START \
	namespace TestUnit\
	{\
		void AddTestUnit(std::vector<std::function<unsigned int(TestConfig& config, TestParameter&)>>& appendToQuery)\
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
		[](TestConfig& testConfig, TestParameter& testParameter) -> unsigned int {	\
	testConfig.m_testName = UnitName;			\
	TestUnit::ErrorLogger errorLogger;		// for each testUnit have a errorLogger to help log the errorCount.

#define TIME_GUARD TimeGuard countTimeOfNextCodes(testParameter.m_timeCounter)
    
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
		\brief give the time unit a name to be printed in the output, e.g. "ms" and the out put may look like '... 45654 ms ...'
	*/
	const char DURATION_TYPE_NAME[] = "us";

    

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

	inline void LogError(size_t errorCount = 1)
	{
		m_errorCount += errorCount;
	}

	// log one error if not equal
	template<typename T1, typename T2>
	inline std::enable_if_t<std::is_same<bool, decltype(std::declval<T1>() != std::declval<T2>())>::value>  // enable if there is an defination of  [ T1 != T2 ]
        LogIfNotEq(const T1& t1, const T2& t2)
	{
		addErrorCount(t1 != t2);
	}

#pragma region specialization
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for float
	inline void LogIfNotEq(const float& t1, const float& t2)
	{
		if (fabs(t1 - t2) > 1e-8f)
		{
			// nearly not equal
			addErrorCount(true);
		}
	}
	// ||||||||||||||||||||||||||||||||||||||||||
	// specialization for double
	inline void LogIfNotEq(const double& t1, const double& t2)
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
    user can reuse it in the code to accumulate the expected time to be recored (NOT the time the whole testUnit costs).
    For example, in one test, you may do some calculations, then write the result into the local file.
    What you concen about is how many time it costs to calculate the result,
    rather than writing result to the local file(writing will be slow if the result is large such as 2GB).
    Below is the case for using the timeCounter:
    {
        TimeGuard _dummy(timeCounter);

        // the codes you want to count time on.
    }// stop on counting

    WARNING! If you use TimeCounter in cascaded, the overlaped duration time will be acummulated.
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
    \brief use RAII to record a period of time and add them to TimeCounter(argument of the constructor).
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
    \brief TestConfig used to 
*/
struct TestConfig
{
public:

    /*!
        \brief the name show in the console.
    */
    std::string m_testName;

    /*!
        \brief loop time for this test unit, the setting is avaliable for the first time running the specific unit.
    */
    unsigned int m_loopTime = 1;

    /*!
        \brief wheter hide the output of this unit test,
        each unit test complete (include the self looping), will output an message which include the time/unitName...
        if you set this to TRUE, the out put will only be an dot to provide some feedback.
    */
    bool        m_hideThisOutput = false;

};

/*!
    \brief the parameters passed into one unit test
    the TestParameter used to pass information or help objects to the inside unit test.
    For example, the m_timeCounter will be provided for user to counting time.
*/
struct TestParameter
{
public:
    /*!
        \brief time counter for the user.
    */
    TimeCounter m_timeCounter;

    /*!
        \brief one unit test can run multiple times, here will store the index of current running test.
    */
    unsigned int m_runningIndex;

    /*!
        \brief if TEST_ASSERT failed, whether trigger normal assertion
    */
    bool         m_breakIfTestAssertionFailed = true;

    TestParameter(unsigned int runningIndex = 0)
        :m_runningIndex(runningIndex)
    {
        // empty
    }
};

/*!
	\brief store brief test result such as test number and success number.
*/
struct TestResult
{
	unsigned int m_countTests = 0;
	unsigned int m_countSuccess = 0;
    unsigned int m_countSkip = 0;
};

/*!
	\brief add test units into the array
	\param appendToQuery append the test units into this array
*/
void AddTestUnit(std::vector<std::function<unsigned int(TestConfig& config, TestParameter&)>>& appendToQuery);

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
inline void RunTest(std::vector<std::function<unsigned int(TestConfig& , TestParameter&)>>& testUnits, TestResult& result)
{
    result.m_countTests = 0;

	for (auto & testUnit : testUnits)
	{
        bool			isSuccess                   = false;
	    unsigned int	errorCountFor_THIS_unitTest;
        bool            isFirstRun                  = true;
        unsigned int    loopIndex                   = 1;        // current loop index.
        /*!
            \brief for each unit test, we can run it multiple times.
            This will be set according to the first setting result of TestConfig whose reference will be passed into the test unit.
        */
        unsigned int    numLoopTime                 = 1;        
        TimeCounter     sumOuterUnitTime;                     // sum all the test run time.
        TimeCounter     sumInnerUnitTime;                   // sum time recored by inside user's codes


        // start the loop of the unit test
        // basically, the loop will only do once,
        // but if the user modify the TestConfig(inside their test codes), the same test may be fired multiple times.
        // the action of modifying is ONLY avaliable for the first time the test unit runs.
        do
        {
            ++result.m_countTests;
            errorCountFor_THIS_unitTest = 0;
            TestConfig      testConfig;
            TestParameter   testParameter;
            TimeCounter     unitTimeCounter;    // count time of one test unit costs

            try
            {
                TimeGuard guard(unitTimeCounter);

                testParameter.m_runningIndex    = loopIndex;   // set loop index
                errorCountFor_THIS_unitTest     = testUnit(testConfig, testParameter);  // run test unit
                ++loopIndex;    // increament the loop index.
            }
            catch (std::exception e)
            {
                std::cout << "\t" << testConfig.m_testName << " :exception happend: " << e.what() << std::endl;
                break;
            }

            isSuccess = (errorCountFor_THIS_unitTest == 0);
            if (isSuccess)
            {
                ++result.m_countSuccess;
            }

            // is the user want to reduce the out put?
            if (testConfig.m_hideThisOutput)
            {
                //std::printf(".");   // output an dot for feedback
            }
            else
            {
                // output detail info include running time and etc.
                std::printf(
                    // success/failed	errorCount	costTime   unitName
                    "\t%s"              "\t%d"      "\t%8lld %s"   "\t%8lld %s"  "\t\t%s\n",
                    isSuccess ? "success" : "failed",
                    errorCountFor_THIS_unitTest,
                    unitTimeCounter.m_sumDuration.count(), DURATION_TYPE_NAME,
                    testParameter.m_timeCounter.m_sumDuration.count(), DURATION_TYPE_NAME,
                    testConfig.m_testName.c_str());
            }
            
            // if it's the first time that the test unit run,
            // try to get loopTime config from the user codes.
            if (isFirstRun)
            {
                // get desired loop time from the unit test.
                // decreased by one, because we have already run it one time.
                numLoopTime = testConfig.m_loopTime - 1;
                isFirstRun = false;
            }

            // for the self looping tests, count the time for all loop,
            // after which we will output some info about the loop result, such as the average of time that each loop costs.
            sumOuterUnitTime.m_sumDuration += unitTimeCounter.m_sumDuration;
            sumInnerUnitTime.m_sumDuration += testParameter.m_timeCounter.m_sumDuration;
        } while ( (numLoopTime--) > 0); // if loopTime greather than one?

        // if loop index greater than two,
        // means that the same unit test have been run multiple times,
        // out put a small information of those loop tests,
        // for example, the average time costs by the each test.
        if (loopIndex > 2)
        {
            // the unit test have run multiple times, output an average times
            const auto sumOuter = sumOuterUnitTime.m_sumDuration.count();
            const auto sumInner = sumInnerUnitTime.m_sumDuration.count();
            std::printf(
                "\t   LoopSummary:"
                "\tSumOuter:%8lld %s"
                "\tSumInner:%8lld %s"
                "\tAveOuter:%8lld %s"
                "\tAveInner:%8lld %s\n", 
                sumOuter,               DURATION_TYPE_NAME,
                sumInner,               DURATION_TYPE_NAME,
                sumOuter / loopIndex,   DURATION_TYPE_NAME,
                sumInner / loopIndex,   DURATION_TYPE_NAME);
        }
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
	std::vector<std::function<unsigned int(TestConfig&, TestParameter&)>> testUnitQuery;
	AddTestUnit(testUnitQuery);

	TestResult result;
	RunTest(testUnitQuery, result);

	Summary(result);

	// wait for user enter.
	std::getchar();
}

}// namespace TestUnit
