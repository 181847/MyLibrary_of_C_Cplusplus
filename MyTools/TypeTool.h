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
	inline static const size_t IDOf()
	{
		static const size_t idOfTheType = ++m_distribID;
		return idOfTheType;
	}
};
//||||||||||||||||||||||||||||||||||||||||||||||||||
// initialize the series id of the specific Series type
template<typename Series>
size_t IDGenerator<Series>::m_distribID = 0;

// BoolCondition used to return the bool based on the CONDITION value.
template<bool CONDITION, bool THEM, bool ELSE>
struct BoolCondition{};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<bool THEN, bool ELSE>
struct BoolCondition<true, THEN, ELSE>
{
	enum { value = THEN };
};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<bool THEN, bool ELSE>
struct BoolCondition<false, THEN, ELSE>
{
	enum { value = ELSE };
};

// struct IsOneOf is used to check whether a type is in the list of types.
// the first parameter is the type to be checked, rest are the typeList.
// Usage: IsOneOf<int, float, char, double>::value => false
//		  IsOneOf<int, float, char, int>::value	   => true.
template<typename TYPE, typename ...TYPE_LIST>
struct IsOneOf {};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<typename TYPE, typename FIRST, typename ...REST>
struct IsOneOf<TYPE, FIRST, REST...>
{
	enum {
		value =
		BoolCondition<std::is_same<TYPE, FIRST>::value,
		true,
		IsOneOf<TYPE, REST...>::value>::value
	};
};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<typename TYPE>
struct IsOneOf<TYPE>
{
	enum { value = false };
};


// TypeContainer to have a set of type to be checked.
template<typename ...TYPE_LIST>
struct TypeContainer{};
//||||||||||||||||||||||||||||||||||||||||||||||||||
// Seperate the set of types to the firstType,
// and the rest to another TypeContainer.
template<typename FIRST, typename ...REST>
struct TypeContainer<FIRST, REST...>
{
	typedef FIRST firstType;
	typedef TypeContainer<REST...> restTypeContainer;
};
//||||||||||||||||||||||||||||||||||||||||||||||||||
// If there is only one type left, 
// the restTypeContainer is the TypeContainer which have no template parameters.
template<typename LAST>
struct TypeContainer<LAST>
{
	typedef LAST firstType;
	typedef TypeContainer<> restTypeContainer;
};


// IsAllOf is the extension of IsOneOf, 
// It will check if all the types are in another typeList.
// It MUST be used with typeContainer.
// Usage:	IsAllOf< TypeContainer<int, char>,			double, int, char	>::value	=> true
//			IsAllOf< TypeContainer<>,					double, int			>::value	=> true
//			IsAllOf< TypeContainer<void>,				double, int			>::value	=> false
//			IsAllOf< TypeContainer<void, double, int>						>::value	=> false
//			IsAllOf<									int, float, double	>::value	=> compile error
template<typename ...CHECK_ARGS>
struct IsAllOf {};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<typename ...TYPE_LIST>
struct IsAllOf<TypeContainer<>, TYPE_LIST...>
{
	enum { value = true };
};
//||||||||||||||||||||||||||||||||||||||||||||||||||
template<typename ...TYPE_LIST_ALL, typename ...TYPE_LIST>
struct IsAllOf<TypeContainer<TYPE_LIST_ALL...>, TYPE_LIST...>
{
	typedef TypeContainer<TYPE_LIST_ALL...> TYPE_CONTAINER;
	
	enum {
		value = IsOneOf<TYPE_CONTAINER::firstType, TYPE_LIST...>::value 
		&& IsAllOf<TYPE_CONTAINER::restTypeContainer, TYPE_LIST...>::value
	};
};


} // namespace TypeTool
