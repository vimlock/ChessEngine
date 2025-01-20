#pragma once

#include "Bitboard.h"
#include <cassert>

namespace vimlock
{

inline constexpr Bitboard::Bitboard():
	bits(0)
{
}

inline constexpr Bitboard::Bitboard(Square square):
	bits(1ULL << square.getIndex())
{
}

inline constexpr Bitboard::Bitboard(RankAndFile index):
	bits(1ULL << static_cast<uint64_t>(index))
{
}

inline constexpr Bitboard::Bitboard(uint64_t bits_):
	bits(bits_)
{
}

inline constexpr Bitboard Bitboard::file(int file)
{
	return Bitboard(0x0101010101010101ULL << file);
}

inline constexpr Bitboard Bitboard::rank(int rank)
{
	return Bitboard(0xFFULL << rank * 8);
}

inline Bitboard Bitboard::adjacent(Square square)
{
	Bitboard center(square);
	Bitboard ret;

	// Left, right
	ret |= (center & ~file(FILE_A)) >> 1;
	ret |= (center & ~file(FILE_H)) << 1;

	// Up, down
	ret |= center << 8;
	ret |= center >> 8;

	// Diagonals
	ret |= (center & ~file(FILE_A)) << 7;
	ret |= (center & ~file(FILE_H)) >> 7;
	ret |= (center & ~file(FILE_H)) << 9;
	ret |= (center & ~file(FILE_A)) >> 9;

	return ret;
}

inline constexpr bool Bitboard::empty() const
{
	return bits == 0;
}

inline constexpr uint64_t Bitboard::rawBits() const
{
	return bits;
}

inline constexpr bool Bitboard::contains(int file, int rank) const
{
	return *this & Bitboard(Square(file, rank));
}

inline constexpr bool Bitboard::contains(Square square) const
{
	return *this & Bitboard(square);
}

inline constexpr bool Bitboard::overlaps(const Bitboard &other) const
{
	return !(*this & other).empty();
}

inline constexpr int Bitboard::count() const
{
#if defined(__GNUC__) || defined(__clang__)
	return __builtin_popcountll(bits);
#else
#warning "no optimized function for popcount"
	int ret = 0;

	for (unsigned i = 0; i < 64; ++i) {
		ret += (bits & (1ULL << i)) ? 1 : 0;
	}

	return ret;
#endif
}

inline constexpr Square Bitboard::findFirstSquare() const
{
#if defined(__GNUC__) || defined(__clang__)
	return Square(__builtin_ffsll(bits));
#else
#warning "no optimized function for ffs"
	for (unsigned i = 0; i < 64; ++i) {
		if (bits & (1ULL << i))
			return i;
	}
	return 0;
#endif
}

inline constexpr Bitboard Bitboard::inverted() const
{
	return Bitboard(~bits);
}

// TODO: This is basically just an endianness flip?
inline Bitboard Bitboard::flipRanks() const
{
	uint64_t ret =
		((bits & 0x00000000000000FFULL) << 56ULL) |
		((bits & 0x000000000000FF00ULL) << 40ULL) |
		((bits & 0x0000000000FF0000ULL) << 24ULL) |
		((bits & 0x00000000FF000000ULL) << 8ULL) |
		((bits & 0x000000FF00000000ULL) >> 8ULL) |
		((bits & 0x0000FF0000000000ULL) >> 24ULL) |
		((bits & 0x00FF000000000000ULL) >> 40ULL) |
		((bits & 0xFF00000000000000ULL) >> 56ULL) ;

	return Bitboard(ret);
}


inline constexpr Bitboard::operator bool () const
{
	return bits;
}

inline constexpr bool Bitboard::operator == (Bitboard rhs) const
{
	return bits == rhs.bits;
}

inline constexpr bool Bitboard::operator != (Bitboard rhs) const
{
	return bits != rhs.bits;
}

inline constexpr Bitboard Bitboard::operator ~ () const
{
	return inverted();
}

inline constexpr Bitboard Bitboard::operator | (Bitboard rhs) const
{
	return Bitboard(bits | rhs.bits);
}

inline Bitboard& Bitboard::operator |= (Bitboard rhs)
{
	return *this = *this | rhs;
}

inline constexpr Bitboard Bitboard::operator & (Bitboard rhs) const
{
	return Bitboard(bits & rhs.bits);
}

inline Bitboard& Bitboard::operator &= (Bitboard rhs)
{
	return *this = *this & rhs;
}

inline constexpr Bitboard Bitboard::operator << (uint64_t nbits) const
{
	return Bitboard(bits << nbits);
}

inline constexpr Bitboard Bitboard::operator << (int nbits) const
{
	return Bitboard(bits << static_cast<uint64_t>(nbits));
}

inline constexpr Bitboard Bitboard::operator >> (uint64_t nbits) const
{
	return Bitboard(bits >> nbits);
}

inline constexpr Bitboard Bitboard::operator >> (int nbits) const
{
	return Bitboard(bits >> static_cast<uint64_t>(nbits));
}

} // namespace vimlock
