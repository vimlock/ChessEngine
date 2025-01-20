#pragma once

#include "Moves.h"
#include <cassert>

namespace vimlock
{

inline Bitboard getAvailableMoves(Color color, Piece piece, Square idx, Bitboard allPieces, Bitboard ownPieces)
{
	Bitboard ret;

	switch (piece) {
		case PAWN:
			ret = getPawnMoves(color, idx, allPieces);
			// Attack only opponent pieces
			ret = ret | (getPawnAttacks(color, idx) & (~ownPieces & allPieces));
			break;
		case ROOK:
			ret = getRookMoves(idx, allPieces);
			break;
		case KNIGHT:
			ret = getKnightMoves(idx);
			break;
		case BISHOP:
			ret = getBishopMoves(idx, allPieces);
			break;
		case QUEEN:
			ret = getQueenMoves(idx, allPieces);
			break;
		case KING:
			ret = getKingMoves(idx);
			break;
		default:
			assert(false && "Should be unreachable");
	}

	// Don't capture our own pieces.
	ret = ret & ~ownPieces;

	return ret;
}


inline Bitboard getAvailableCaptures(Color color, Piece piece, Square idx, Bitboard allPieces)
{
	switch (piece) {
		case PAWN:
			return getPawnAttacks(color, idx);
		case ROOK:
			return getRookMoves(idx, allPieces);
		case KNIGHT:
			return getKnightMoves(idx);
		case BISHOP:
			return getBishopMoves(idx, allPieces);
		case QUEEN:
			return getQueenMoves(idx, allPieces);
		case KING:
			return getKingMoves(idx);
	}

	// Empty square?
	return Bitboard();
}

inline Bitboard getAvailableCaptures(const Board &board, Bitboard allPieces, Bitboard ownPieces)
{
	Bitboard ret;

	for (uint64_t i = 0; i < 64; ++i) {
		if (ownPieces & (Bitboard(Square(i)))) {
			SquareState square = board.getSquare(Square(i));
			ret |= getAvailableCaptures(square.getColor(), square.getPiece(), Square(i), allPieces);
		}
	}

	return ret;
}

inline Bitboard getPawnMoves(Color color, Square idx, Bitboard allPieces)
{
	Bitboard ret;

	int rank = idx.getRank();
	int file = idx.getFile();
	int dir = color == WHITE ? +1 : -1;

	// Can move forward one step?
	if (!allPieces.contains(file, rank + dir)) {
		ret |= Bitboard(Square(file, rank + dir));

		// Can perform double move? Only valid if we can move at least one step

		if (color == WHITE && rank == RANK_2) {
			if (!allPieces.contains(file, rank + 2)) {
				ret |= Bitboard(Square(file, rank + 2));
			}
		}

		if (color == BLACK && rank == RANK_7) {
			if (!allPieces.contains(file, rank - 2)) {
				ret |= Bitboard(Square(file, rank - 2));
			}
		}
	}

	return ret;
}

// TODO: en-passant
inline Bitboard getPawnAttacks(Color color, Square idx)
{
	Bitboard ret;

	int rank = idx.getRank();
	int file = idx.getFile();
	int dir = color == WHITE ? +1 : -1;

	// Can capture a left diagonal piece?
	if (file > FILE_A) {
		ret |= Bitboard(Square(file - 1, rank + dir));
	}

	// Can capture a right diagonal piece?
	if (file < FILE_H) {
		ret |= Bitboard(Square(file + 1, rank + dir));
	}

	return ret;
}

inline Bitboard getRookMoves(Square idx, Bitboard allPieces)
{
	Bitboard ret;

	int rank = idx.getRank();
	int file = idx.getFile();

	// Right movement
	for (int i = 1; file + i <= FILE_H; ++i) {

		Bitboard dst(Square(file + i, rank));
		ret |= dst;

		// Blocked by something?
		if (allPieces & dst) {
			break;
		}
	}

	// Left movement
	for (int i = 1; file - i >= FILE_A; ++i) {

		Bitboard dst(Square(file - i, rank));
		ret |= dst;

		// Blocked by something?
		if (allPieces & dst) {
			break;
		}
	}

	// Upward movement
	for (int i = 1; rank + i <= RANK_8; ++i) {

		Bitboard dst(Square(file, rank + i));
		ret |= dst;
		
		// Blocked by something?
		if (allPieces & dst) {
			break;
		}

	}

	// Downward movement
	for (int i = 1; rank - i >= RANK_1; ++i) {
		Bitboard dst(Square(file, rank - i));
		ret |= dst;

		// Blocked by something?
		if (allPieces & dst) {
			break;
		}

	}

	return ret;
}

inline Bitboard getKnightMoves(Square idx)
{
	Bitboard ret;
	Bitboard pos(idx);

	// Up 2, right 1
	ret |= (pos << (8*2+1)) & ~Bitboard::file(FILE_A);
	// Up 2, left 1 
	ret |= (pos << (8*2-1)) & ~Bitboard::file(FILE_H);
	// Down 2, right 1
	ret |= (pos >> (8*2-1)) & ~Bitboard::file(FILE_A);
	// Down 2, left 1
	ret |= (pos >> (8*2+1)) & ~Bitboard::file(FILE_H);

	// Right 2, up 1
	ret |= (pos << (8*1+2)) & ~(Bitboard::file(FILE_A) | Bitboard::file(FILE_B));
	// Right 2, down 1
	ret |= (pos >> (8*1-2)) & ~(Bitboard::file(FILE_A) | Bitboard::file(FILE_B));
	// Left 2, up 1
	ret |= (pos << (8*1-2)) & ~(Bitboard::file(FILE_G) | Bitboard::file(FILE_H));
	// Left 2, down 1
	ret |= (pos >> (8*1+2)) & ~(Bitboard::file(FILE_G) | Bitboard::file(FILE_H));

	return ret;
}


inline Bitboard getBishopMoves(Square idx, Bitboard allPieces)
{
	Bitboard ret;
	Bitboard tmp;
	Bitboard pos(idx);

	// Up left diagonal
	tmp = pos;
	while (tmp) {
		tmp = (tmp << (8-1)) & ~Bitboard::file(FILE_H);
		ret |= tmp;

		if (allPieces & tmp)
			break;
	}

	// Up right diagonal
	tmp = pos;
	while (tmp) {
		tmp = (tmp << (8+1)) & ~Bitboard::file(FILE_A);
		ret |= tmp;
		if (allPieces & tmp)
			break;
	}

	// Down left diagonal
	tmp = pos;
	while (tmp) {
		tmp = (tmp >> (8-1)) & ~Bitboard::file(FILE_A);
		ret |= tmp;
		if (allPieces & tmp)
			break;
	}

	// Down right diagonal
	tmp = pos;
	while (tmp) {
		tmp = (tmp >> (8+1)) & ~Bitboard::file(FILE_H);
		ret |= tmp;
		if (allPieces & tmp)
			break;
	}

	return ret;
}

inline Bitboard getQueenMoves(Square idx, Bitboard allPieces)
{
	Bitboard ret;

	ret |= getBishopMoves(idx, allPieces);
	ret |= getRookMoves(idx, allPieces);

	return ret;
}

inline Bitboard getKingMoves(Square idx)
{
	Bitboard ret;
	Bitboard pos(idx);

	// Up
	ret |= (pos << 8); // will truncate if we're at top rank
	// Down
	ret |= (pos >> 8); // will truncate if we're at bottom rank
	// Left
	ret |= (pos >> 1) & ~Bitboard::file(FILE_H);
	// Right
	ret |= (pos << 1) & ~Bitboard::file(FILE_A);
	// Up Left
	ret |= (pos << (8-1)) & ~Bitboard::file(FILE_H);
	// Up Right
	ret |= (pos << (8+1)) & ~Bitboard::file(FILE_A);
	// Down Left
	ret |= (pos >> (8+1)) & ~Bitboard::file(FILE_H);
	// Down Right
	ret |= (pos >> (8-1)) & ~Bitboard::file(FILE_A);

	return ret;
}

} // namespace vimlock
