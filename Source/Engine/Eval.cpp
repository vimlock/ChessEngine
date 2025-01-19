#include "Eval.h"
#include "Board.h"

#include <cassert>
#include <iostream>

namespace vimlock
{

Color BoardUtils::getOpponent(Color color)
{
	assert(color == WHITE || color == BLACK);

	if (color == WHITE)
		return BLACK;
	else
		return WHITE;
}


MoveEval::MoveEval(const Board &board_, Color color):
	board(board_)
{
	ownPieces = getPieces(color);
	oppPieces = getPieces(BoardUtils::getOpponent(color));
	allPieces = ownPieces | oppPieces;
	ownKing = getPieces(color, KING);

	// Cache squares where opponent is attacking.
	for (uint64_t i = 0; i < 64; ++i) {
		if (oppPieces & (Bitboard(Square(i)))) {
			attackedSquares |= getAvailableCaptures(Square(i));
		}
	}

	// Cache squares where we are attacking.
	for (uint64_t i = 0; i < 64; ++i) {
		if (ownPieces & (Bitboard(Square(i)))) {
			attackingSquares |= getAvailableCaptures(Square(i));
		}
	}

	// Cache squares where we can move to
	for (uint64_t i = 0; i < 64; ++i) {
		if (ownPieces & (Bitboard(Square(i)))) {
			ownAvailableMoves |= getAvailableMoves(Square(i));
		}
	}
}

Bitboard MoveEval::getAvailableMoves(Square idx) const
{
	SquareState square = board.getSquare(idx);
	Bitboard ret;

	switch (square.getPiece()) {
		case PAWN:
			ret = getPawnMoves(square.getColor(), idx);

			// Prevent attack to where there is no enemy pieces
			ret |= oppPieces & getPawnAttacks(square.getColor(), idx);
			break;
		case ROOK:
			ret = getRookMoves(idx);
			break;
		case KNIGHT:
			ret = getKnightMoves(idx);
			break;
		case BISHOP:
			ret = getBishopMoves(idx);
			break;
		case QUEEN:
			ret = getQueenMoves(idx);
			break;
		case KING:
			ret = getKingMoves(idx);
			break;
		default:
			assert(false && "Should be unreachable");
	}

	// Don't capture our own pieces...
	ret = ret & ~ownPieces;

	return ret;
}

Bitboard MoveEval::getAvailableCaptures(Square idx) const
{
	SquareState square = board.getSquare(idx);

	switch (square.getPiece()) {
		case PAWN:
			return getPawnAttacks(square.getColor(), idx);
		case ROOK:
			return getRookMoves(idx);
		case KNIGHT:
			return getKnightMoves(idx);
		case BISHOP:
			return getBishopMoves(idx);
		case QUEEN:
			return getQueenMoves(idx);
		case KING:
			return getKingMoves(idx);
	}

	// Empty square?
	return Bitboard();
}


Bitboard MoveEval::getPieces(Color color) const
{
	Bitboard ret;

	for (uint64_t i = 0; i < 64; ++i) {
		SquareState square = board.getSquare(Square(i));
		ret |= Bitboard(((square.getBits() & color) ? 1ULL : 0ULL) << i);
	}

	return ret;
}

Bitboard MoveEval::getPieces(Piece piece) const
{
	Bitboard ret;

	for (uint64_t i = 0; i < 64; ++i) {
		SquareState square = board.getSquare(Square(i));
		ret |= Bitboard(((square.getBits() & piece) ? 1ULL : 0ULL) << i);
	}

	return ret;
}

Bitboard MoveEval::getPieces(Color color, Piece piece) const
{
	Bitboard ret;
	uint8_t mask  = color | piece;

	for (uint64_t i = 0; i < 64; ++i) {
		SquareState square = board.getSquare(Square(i));
		ret |= Bitboard((((square.getBits() & mask) == mask) ? 1ULL : 0ULL) << i);
	}

	return ret;
}

Bitboard MoveEval::getOwnPieces() const
{
	return ownPieces;
}

Bitboard MoveEval::getOppPieces() const
{
	return oppPieces;
}

Bitboard MoveEval::getAttackedSquares() const
{
	return attackedSquares;
}

Bitboard MoveEval::getAttackingSquares() const
{
	return attackingSquares;
}

Bitboard MoveEval::getOwnAllAvailableMoves() const
{
	return ownAvailableMoves;
}

bool MoveEval::isInCheck() const
{
	return attackedSquares & ownKing;
}

Bitboard MoveEval::getPawnMoves(Color color, Square idx) const
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
Bitboard MoveEval::getPawnAttacks(Color color, Square idx) const
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

Bitboard MoveEval::getRookMoves(Square idx) const
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

Bitboard MoveEval::getKnightMoves(Square idx) const
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

Bitboard MoveEval::getBishopMoves(Square idx) const
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

Bitboard MoveEval::getQueenMoves(Square idx) const
{
	Bitboard ret;

	ret |= getBishopMoves(idx);
	ret |= getRookMoves(idx);

	return ret;
}

Bitboard MoveEval::getKingMoves(Square idx) const
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
