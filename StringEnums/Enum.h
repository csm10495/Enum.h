/*
Enum.h - Really simple, non-optimized preprocessor macros for creating enums with functions
	to get string representations of the enum values. This is an abuse of the preprocessor.
MIT License - Charles Machalow - 2018
*/
#pragma once

#include <cstdlib>
#include <map>
#include <vector>
#include <set>
#include <string>

#define ASSERT_AND_PRINT(thing) \
if (!(thing)) \
	{ \
		std::cout << "c++ assertion failure " << __FILE__ << ":" << __LINE__ << "> " << #thing << std::endl; \
		assert(0); \
	}

inline std::vector<std::string> splitString(std::string start, char splitOn)
{
	std::vector<std::string> retVec;
	size_t currentStart = 0;
	for (size_t i = 0; i < start.size(); i++)
	{
		if (start[i] == splitOn)
		{
			retVec.push_back(start.substr(currentStart, i - currentStart));
			i++;
			currentStart = i;
		}
	}

	// don't forget the last part
	if (currentStart != start.size())
	{
		retVec.push_back(start.substr(currentStart, start.size() - currentStart));
	}
	return retVec;
}

inline std::string& trim(std::string& str)
{
	for (size_t i = str.size() - 1; i != (size_t)-1; i--)
	{
		if (str[i] == ' ')
		{
			str = str.erase(i, 1);
		}
	}
	return str;
}

template <typename TYPE>
inline bool getValueFromStringName(const std::map<TYPE, std::set<std::string>> &toStringMap, std::string str, TYPE &passed)
{
	for (auto itr = toStringMap.begin(); itr != toStringMap.end(); itr++)
	{
		auto &setOfStrings = itr->second;
		if (setOfStrings.find(str) != setOfStrings.end())
		{
			passed = itr->first;
			return true;
		}
	}
	return false;
}

inline std::string getStringFromSetOfStrings(const std::set<std::string> &set)
{
	std::string retStr = "";
	for (auto itr = set.begin(); itr != set.end(); itr++)
	{
		if (retStr.size())
		{
			retStr += "/";
		}
		retStr += *itr;
	}
	return retStr;
}

#ifdef DEBUG_ENUM
#define CREATE_GLOBAL_DEBUG_MAP(TYPE) std::map<TYPE, std::set<std::string>> debugToStringMap;
#define SAVE_DEBUG_MAP(MAP) debugToStringMap = MAP;
#define DEBUG_PRINT_VALUE(THING) std::cout << #THING << ": " << THING << std::endl;
#define DEBUG_PRINT(THING) std::cout << THING << std::endl;
#else
#define SAVE_DEBUG_MAP(MAP)
#define CREATE_GLOBAL_DEBUG_MAP(TYPE)
#define DEBUG_PRINT_VALUE(THING)
#define DEBUG_PRINT(THING)
#endif

#define MAKE_TYPED_ENUM(NAME, TYPE, ...) \
typedef enum _##NAME : TYPE{ \
	__VA_ARGS__ \
} NAME; \
CREATE_GLOBAL_DEBUG_MAP(TYPE); \
const inline std::string getString##NAME(NAME passedValue) \
{ \
	static std::map<TYPE, std::set<std::string>> toStringMap; \
	if (toStringMap.size() == 0) \
	{ \
		std::vector<std::string> splitOnComma = splitString(#__VA_ARGS__, ','); \
		TYPE count = 0; \
		for (auto itr = splitOnComma.begin(); itr != splitOnComma.end(); itr++) \
		{ \
			DEBUG_PRINT("---\n"); \
			std::vector<std::string> splitOnEqual = splitString(*itr, '='); \
			bool found = false; \
			if (splitOnEqual.size() == 0) { continue; }; \
			TYPE value = count; \
			ASSERT_AND_PRINT(splitOnEqual.size() == 1 || splitOnEqual.size() == 2); \
			if (splitOnEqual.size() == 2) \
			{ \
				trim(splitOnEqual[1]); \
				/* If there is a matching string, take its value */ \
				found = getValueFromStringName(toStringMap, splitOnEqual[1], value); \
				if (!found) \
				{ \
					char * pEnd = NULL; \
					char * right = (char*)splitOnEqual[1].c_str(); \
					value = (TYPE)std::strtoull(right, &pEnd, 0); \
					/* Make sure that the right was evaluatable to a numeric*/ \
					ASSERT_AND_PRINT(value != 0 && pEnd != 0); \
				} \
				count = value; \
			} \
			auto stringMapItr = toStringMap.find(value); \
			if (stringMapItr == toStringMap.end()) \
			{ \
				toStringMap[value] = std::set<std::string>(); \
			} \
			toStringMap[value].insert(trim(splitOnEqual[0])); \
			count++; \
			DEBUG_PRINT_VALUE(*itr); \
			DEBUG_PRINT_VALUE(splitOnEqual.size()); \
			DEBUG_PRINT_VALUE(splitOnEqual[0]); \
			if (splitOnEqual.size() == 2) { DEBUG_PRINT_VALUE(splitOnEqual[1]); } \
			DEBUG_PRINT_VALUE(found); \
			DEBUG_PRINT_VALUE(value); \
			DEBUG_PRINT_VALUE(count); \
		} \
	} \
	SAVE_DEBUG_MAP(toStringMap); \
	auto itr = toStringMap.find((TYPE)passedValue); \
	if (itr == toStringMap.end()) \
	{ \
		return "<Unknown Value>"; \
	} \
	return getStringFromSetOfStrings(itr->second); \
} \
const inline std::string getString##NAME(TYPE passedValue) \
{ \
	return getString##NAME((NAME)passedValue); \
}

#define MAKE_ENUM(NAME, ...)  MAKE_TYPED_ENUM(NAME, int, __VA_ARGS__)

/*

//Examples:
MAKE_TYPED_ENUM(CharliesEnum, char,
	A,
	B,
	C = 10);

MAKE_ENUM(CharliesEnum2,
	AA,
	BB);
*/
