#ifndef __BITBOARD_INL__
#define __BITBOARD_INL__
#pragma once

#include "Bitboard.h"
#include <cassert>

namespace vimlock
{

inline Bitboard::Bitboard():
	bits(0)
{
}

inline Bitboard::Bitboard(Square square)
{
	bits = 1ULL << square.getIndex();
}

inline Bitboard::Bitboard(RankAndFile index)
{
	bits = 1ULL << static_cast<uint64_t>(index);
}

inline Bitboard::Bitboard(uint64_t bits_):
	bits(bits_)
{
}

inline Bitboard Bitboard::file(int file)
{
	assert(file >= 0 && file < 8);
	return Bitboard(0x0101010101010101ULL << file);
}

inline Bitboard Bitboard::rank(int rank)
{
	assert(rank >= 0 && rank < 8);
	return Bitboard(0xFFULL << rank * 8);
}

inline bool Bitboard::empty() const
{
	return bits == 0;
}

inline uint64_t Bitboard::rawBits() const
{
	return bits;
}

inline bool Bitboard::contains(int file, int rank) const
{
	return *this & Bitboard(Square(file, rank));
}

inline bool Bitboard::contains(Square square) const
{
	return *this & Bitboard(square);
}

inline bool Bitboard::overlaps(const Bitboard &other) const
{
	return !(*this & other).empty();
}

inline int Bitboard::count() const
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

inline Bitboard Bitboard::inverted() const
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


inline Bitboard::operator bool () const
{
	return bits;
}

inline bool Bitboard::operator == (Bitboard rhs) const
{
	return bits == rhs.bits;
}

inline bool Bitboard::operator != (Bitboard rhs) const
{
	return bits != rhs.bits;
}

inline Bitboard Bitboard::operator ~ () const
{
	return inverted();
}

inline Bitboard Bitboard::operator | (Bitboard rhs) const
{
	return Bitboard(bits | rhs.bits);
}

inline Bitboard& Bitboard::operator |= (Bitboard rhs)
{
	return *this = *this | rhs;
}

inline Bitboard Bitboard::operator & (Bitboard rhs) const
{
	return Bitboard(bits & rhs.bits);
}

inline Bitboard& Bitboard::operator &= (Bitboard rhs)
{
	return *this = *this & rhs;
}

inline Bitboard Bitboard::operator << (uint64_t nbits) const
{
	return Bitboard(bits << nbits);
}

inline Bitboard Bitboard::operator << (int nbits) const
{
	return Bitboard(bits << static_cast<uint64_t>(nbits));
}

inline Bitboard Bitboard::operator >> (uint64_t nbits) const
{
	return Bitboard(bits >> nbits);
}

inline Bitboard Bitboard::operator >> (int nbits) const
{
	return Bitboard(bits >> static_cast<uint64_t>(nbits));
}

} // namespace vimlock

#endif // __BITBOARD_INL__
