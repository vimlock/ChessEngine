#pragma once

#include "Enums.h"
#include <cstdint>

namespace vimlock
{

class Square
{
public:
	/// Initialize this square to A1
	constexpr Square();

	/// Construct a square from file and rank.
	/// Both file and rank must be between 0 and 7
	constexpr Square(int file, int rank);

	/// Assign a raw square index.
	/// Board squares are ordered from A1, A2, A3... all the way to H8
	constexpr Square(uint64_t index);

	/// Construct from an enum.
	constexpr Square(RankAndFile e);

	/// Return file this square is in.
	constexpr int getFile() const;

	/// Return rank this square is in.
	constexpr int getRank() const;

	/// Return index of this square.
	constexpr uint64_t getIndex() const { return index; }

	/// Equality operators
	constexpr bool operator == (Square rhs) const { return index == rhs.index; }
	/// Equality operators
	constexpr bool operator != (Square rhs) const { return index != rhs.index; }

private:
	/// Stored as 64-bit to avoid unnecessary casting when used with Bitboard
	uint64_t index;
};

} // namespace vimlock
  
#include "Square.inl"
