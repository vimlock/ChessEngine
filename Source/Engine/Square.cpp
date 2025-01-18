#include "Square.h"

#include <cassert>

namespace vimlock
{

Square::Square():
	index(0)
{
}

Square::Square(int file, int rank)
{
	assert(file >= 0 && file < 8);
	assert(rank >= 0 && rank < 8);

	index = (rank * 8 + file);
}

Square::Square(uint64_t index_)
{
	assert(index_ >= 0 && index_ < 64);
	index = index_;
}

Square::Square(RankAndFile e)
{
	index = static_cast<uint64_t>(e);
}

int Square::getFile() const
{
	return index % 8;
}

int Square::getRank() const
{
	return index / 8;
}
	
} // namespace vimlock
