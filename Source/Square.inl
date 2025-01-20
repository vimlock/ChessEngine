#pragma once

#include "Square.h"
#include <cassert>

namespace vimlock
{

inline Square::Square():
	index(0)
{
}

inline Square::Square(int file, int rank)
{
	assert(file >= 0 && file < 8);
	assert(rank >= 0 && rank < 8);

	index = (rank * 8 + file);
}

inline Square::Square(uint64_t index_)
{
	assert(index_ >= 0 && index_ < 64);
	index = index_;
}

inline Square::Square(RankAndFile e)
{
	index = static_cast<uint64_t>(e);
}

inline int Square::getFile() const
{
	return index % 8;
}

inline int Square::getRank() const
{
	return index / 8;
}
	
} // namespace vimlock
