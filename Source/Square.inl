#pragma once

#include "Square.h"

namespace vimlock
{

inline constexpr Square::Square():
	index(0)
{
}

inline constexpr Square::Square(int file, int rank):
	index(rank * 8 + file)
{
}

inline constexpr Square::Square(uint64_t index_):
	index(index_)
{
}

inline constexpr Square::Square(RankAndFile e):
	index(static_cast<uint64_t>(e))
{
}

inline constexpr int Square::getFile() const
{
	return index % 8;
}

inline constexpr int Square::getRank() const
{
	return index / 8;
}

inline std::string Square::toString() const
{
	std::string ret;
	ret += static_cast<char>('a' + getFile());
	ret += static_cast<char>('1' + getRank());
	return ret;
}
	
} // namespace vimlock
