#pragma once
#include "Square.h"

namespace vimlock
{

/// Represents various board states as a 64-bit mask,
/// e.g. where a piece can move, which squares are attack, etc.
class Bitboard
{
public:
	/// Construct a bitboard with all bits set to 0
	explicit Bitboard();

	/// Construct a bitboard with given square set to 1
	explicit Bitboard(Square square);

	/// Construct a bitboard with given square set to 1
	explicit Bitboard(RankAndFile index);

	/// Construct a bitboard with all bits assigned
	explicit Bitboard(uint64_t bits);

	/// Construct a bitboard with given file filled.
	static Bitboard file(int file);

	/// Construct a bitboard with given rank filled.
	static Bitboard rank(int rank);

	/// Returns true if all bits are 0.
	bool empty() const;

	/// Returns true if given bit on file and rank is set.
	bool contains(int file, int rank) const;
	bool contains(Square square) const;

	/// Returns true if the bitboards have overlapping bits
	bool overlaps(const Bitboard &other) const;

	/// Returns total number of bits set on the board.
	int count() const;

	/// Return board withb bits flipped, same as ~ operator.
	Bitboard inverted() const;

	/// Return raw bit layout.
	uint64_t rawBits() const;

	/// Flip the board rank around, rank 1 becomes 8 and vice versa.
	Bitboard flipRanks() const;

	/// If any bit is set returns true, otherwise returns false.
	operator bool () const;

	/// Returns inverted version of this bitboard with squares flipped.
	Bitboard operator ~ () const;

	// Strict equality comparison
	bool operator == (Bitboard rhs) const;
	bool operator != (Bitboard rhs) const;

	/// Return logical OR between 2 bitboards.
	Bitboard operator | (Bitboard rhs) const;
	Bitboard& operator |= (Bitboard rhs);

	/// Return logical AND between 2 bitboards.
	Bitboard operator & (Bitboard rhs) const;
	Bitboard& operator &= (Bitboard rhs);

	// Shift all bits to left by `nbits`.
	Bitboard operator << (uint64_t nbits) const;
	Bitboard operator << (int nbits) const;

	// Shift all bits to right by `nbits`.
	Bitboard operator >> (uint64_t nbits) const;
	Bitboard operator >> (int nbits) const;

private:
	uint64_t bits;
};

} // namespace vimlock
  
#include "Bitboard.inl"
