#pragma once


namespace TypeTool
{

// this struct is used to generate the id for specific type,
// the id is unique for specific SERIES_TYPE,
// you can change the SERIES_TYPE to get other series id.
// the ID start from one,
// and the zero means a invalid ID.
template<typename SERIES_TYPE>
struct IDGenerator
{

private:
	// store the current max id,
	// before called newID(), it should be 0.
	static size_t m_distribID;

public:
	template<typename TYPE>
	inline static size_t IDOf()
	{
		static const size_t idOfTheType = ++m_distribID;
		return idOfTheType;
	}
};


// initialize the series id of the specific Series type
template<typename Series>
size_t IDGenerator<Series>::m_distribID = 0;


} // namespace TypeIDTools
