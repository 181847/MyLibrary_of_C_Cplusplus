#pragma once


namespace TypeTool
{

// this struct is used to generate the id for specific type,
// the id is unique for specific ParentType,
// you can change the SERIES_TYPE to get other series id.
// the ID start from one,
// and the zero means a invalid ID.
template<typename SERIES_TYPE>
struct IDGenerator
{

private:
	// store the current max id,
	// before called newID(), it should be 0.
	static size_t distribID;

	// this function used as a IDContainer to
	// return the ID for specific type and a ID setter
	// at the same time.
	// It is private, can prevent the user to change the id.
	// It will be called in the newID(), and the getID(),
	// previous one will pass in a desiredID which is valid ( >= 1),
	// but the later one will not pass in the arguments (by default = zero).
	// if you look at the idForTheType which is static and be 
	// initialized by the argument,
	// this means the newID() must be called before thr getID(),
	template<typename TYPE>
	inline static size_t IDContainer(size_t desiredID = 0)
	{
		static size_t idForTheType = desiredID;
		return idForTheType;
	}
	
public:
	// if the Type is new, it will generate a new ID and return it,
	// if the Type has been called, just return the previous id, and no new id generate.
	template<typename TYPE>
	inline static size_t newID()
	{
		static const size_t idOfTheType = IDContainer<TYPE>(++distribID);
		return idOfTheType;
	}

	// Get the id of the Type,
	// if the Type is new, unlike the newID(),
	// it won't generate any new ID, and will return zero all the time
	// with the same Type(even you call newID() later)
	// so this mechanism can make the user clearly understand 
	// whether they want to generate newID, 
	// or just to try a Type
	// and push them to call newID() first,
	// then getID(),
	// rather than create ID all the time.
	template<typename TYPE>
	inline static size_t getID()
	{
		static size_t idOfTheType = IDContainer<TYPE>(0);
		return idOfTheType;
	}

	template<typename TYPE>
	inline static size_t IDOf()
	{
		static const size_t idOfTheType = IDContainer<TYPE>(++distribID);
		return idOfTheType;
	}
};


// initialize the series id of the specific Series type
template<typename Series>
size_t IDGenerator<Series>::distribID = 0;


} // namespace TypeIDTools
