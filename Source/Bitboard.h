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
	explicit constexpr Bitboard();

	/// Construct a bitboard with given square set to 1
	explicit constexpr Bitboard(Square square);

	/// Construct a bitboard with given square set to 1
	explicit constexpr Bitboard(RankAndFile index);

	/// Construct a bitboard with all bits assigned
	explicit constexpr Bitboard(uint64_t bits);

	/// Construct a bitboard with given file filled.
	static constexpr Bitboard file(int file);

	/// Construct a bitboard with given rank filled.
	static constexpr Bitboard rank(int rank);

	/// Construct a bitboard with adjacent squares around center filled
	static Bitboard adjacent(Square square);

	/// Returns true if all bits are 0.
	constexpr bool empty() const;

	/// Returns true if given bit on file and rank is set.
	constexpr bool contains(int file, int rank) const;
	constexpr bool contains(Square square) const;

	/// Returns true if the bitboards have overlapping bits
	constexpr bool overlaps(const Bitboard &other) const;

	/// Returns first available square with a bit set.
	constexpr Square findFirstSquare() const; 

	/// Returns total number of bits set on the board.
	constexpr int count() const;

	/// Return board withb bits flipped, same as ~ operator.
	constexpr Bitboard inverted() const;

	/// Return raw bit layout.
	constexpr uint64_t rawBits() const;

	/// Flip the board rank around, rank 1 becomes 8 and vice versa.
	Bitboard flipRanks() const;

	/// If any bit is set returns true, otherwise returns false.
	constexpr operator bool () const;

	/// Returns inverted version of this bitboard with squares flipped.
	constexpr Bitboard operator ~ () const;

	// Strict equality comparison
	constexpr bool operator == (Bitboard rhs) const;
	constexpr bool operator != (Bitboard rhs) const;

	/// Return logical OR between 2 bitboards.
	constexpr Bitboard operator | (Bitboard rhs) const;
	Bitboard& operator |= (Bitboard rhs);

	/// Return logical AND between 2 bitboards.
	constexpr Bitboard operator & (Bitboard rhs) const;
	Bitboard& operator &= (Bitboard rhs);

	// Shift all bits to left by `nbits`.
	constexpr Bitboard operator << (uint64_t nbits) const;
	constexpr Bitboard operator << (int nbits) const;

	// Shift all bits to right by `nbits`.
	constexpr Bitboard operator >> (uint64_t nbits) const;
	constexpr Bitboard operator >> (int nbits) const;

private:
	uint64_t bits;
};

} // namespace vimlock
  
#include "Bitboard.inl"
