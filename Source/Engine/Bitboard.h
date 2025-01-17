#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <cstdint>

namespace vimlock
{

/// Represents various board states as a 64-bit mask,
/// e.g. where a piece can move, which squares are attack, etc.
class Bitboard
{
public:
	/// Construct a bitboard with all bits set to 0
	explicit Bitboard();

	/// Construct a bitboard with given bit set to 1
	explicit Bitboard(int file, int rank);

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

	/// Returns inverted version of this bitboard with squares flipped.
	Bitboard operator ~ () const;

	/// Return logical OR between 2 bitboards.
	Bitboard operator | (Bitboard rhs) const;
	Bitboard& operator |= (Bitboard rhs);

	/// Return logical AND between 2 bitboards.
	Bitboard operator & (Bitboard rhs) const;
	Bitboard& operator &= (Bitboard rhs);

	// Shift all bits to left by `nbits`.
	Bitboard operator << (uint64_t nbits) const;

	// Shift all bits to right by `nbits`.
	Bitboard operator >> (uint64_t nbits) const;

private:
	uint64_t bits;
};

} // namespace vimlock

#endif // __BITBOARD_H__
