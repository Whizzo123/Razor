#pragma once

inline int32_t Size_tToInt(size_t Variable)
{
	const int VariableSize = sizeof(Variable);
	const int MaxInt32_tSize = sizeof(sizeof(std::numeric_limits<int32_t>::max()));
	if (VariableSize > MaxInt32_tSize)
	{ 
		throw std::overflow_error("Input string is too long: size_t-length doesn't fit into int");
	} 
	return static_cast<int>(Variable);
}