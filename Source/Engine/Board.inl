#ifndef __BOARD_INL__
#define __BOARD_INL__
#pragma once

#include "Board.h"

#include <cassert>

namespace vimlock
{

inline SquareState::SquareState():
	bits(0)
{
}

inline SquareState::SquareState(Color color, Piece piece):
	bits(color | piece)
{
}

inline bool SquareState::isOccupied() const
{
	return bits & PIECE_MASK;
}

inline Color SquareState::getColor() const
{
	assert(isOccupied());
	return static_cast<Color>(bits & COLOR_MASK);
}

inline Piece SquareState::getPiece() const
{
	assert(isOccupied());
	return static_cast<Piece>(bits & PIECE_MASK);
}

inline uint8_t SquareState::getBits() const
{
	return bits;
}

inline void Board::setSquare(Square idx, Color color, Piece piece)
{
	setSquare(idx, SquareState(color, piece));
}

inline void Board::setSquare(RankAndFile idx, Color color, Piece piece)
{
	setSquare(Square(idx), SquareState(color, piece));
}

inline void Board::setSquare(Square idx, SquareState square)
{
	squares[idx.getIndex()] = square;
}

inline SquareState Board::getSquare(Square idx) const
{
	return squares[idx.getIndex()];
}

inline SquareState Board::getSquare(int file, int rank) const
{
	return getSquare(Square(file, rank));
}

inline Color Board::getCurrent() const
{
	return current;
}

inline void Board::setCurrent(Color color)
{
	current = color;
}

} // namespace vimlock

#endif // __BOARD_INL__
