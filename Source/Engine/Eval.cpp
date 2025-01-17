#include "Eval.h"
#include "Board.h"

#include <cassert>
#include <iostream>

namespace vimlock
{

Bitboard BoardUtils::getPieces(const Board &board, Color color)
{
	Bitboard ret;

	for (uint64_t i = 0; i < 64; ++i) {
		Square square = board.getSquare(static_cast<RankAndFile::Enum>(i));
		ret |= Bitboard(((square.getBits() & color) ? 1ULL : 0ULL) << i);
	}

	return ret;
}

Bitboard BoardUtils::getPieces(const Board &board, Piece piece)
{
	Bitboard ret;

	for (uint64_t i = 0; i < 64; ++i) {
		Square square = board.getSquare(static_cast<RankAndFile::Enum>(i));
		ret |= Bitboard(((square.getBits() & piece) ? 1ULL : 0ULL) << i);
	}

	return ret;
}


int BoardUtils::getRank(RankAndFile::Enum raf)
{
	return static_cast<int>(raf) / 8;
}

int BoardUtils::getFile(RankAndFile::Enum raf)
{
	return static_cast<int>(raf) % 8;
}

MoveEval::MoveEval(const Board &board_):
	board(board_)
{
	ownPieces = BoardUtils::getPieces(board, board.getCurrent());
	oppPieces = BoardUtils::getPieces(board, getOpponent(board.getCurrent()));
	allPieces = ownPieces | oppPieces;
}

Color MoveEval::getOpponent(Color color) const
{
	assert(color == WHITE || color == BLACK);

	if (color == WHITE)
		return BLACK;
	else
		return WHITE;
}

Bitboard MoveEval::getAvailableMoves(RankAndFile::Enum raf) const
{
	Square square = board.getSquare(raf);

	switch (square.getPiece()) {
		case PAWN:
			return getPawnMoves(square.getColor(), raf);
		case ROOK:
			return getRookMoves(raf);
		case KNIGHT:
			return getKnightMoves(raf);
		case BISHOP:
			return getBishopMoves(raf);
		case QUEEN:
			return getQueenMoves(raf);
		case KING:
			return getKingMoves(square.getColor(), raf);
	}

	// Empty square?
	return Bitboard();
}

// TODO: en-passant
Bitboard MoveEval::getPawnMoves(Color color, RankAndFile::Enum raf) const
{
	Bitboard ret;

	int rank = BoardUtils::getRank(raf);
	int file = BoardUtils::getFile(raf);
	int dir = color == WHITE ? +1 : -1;

	// Can move forward one step?
	if (!allPieces.contains(file, rank + dir)) {
		ret |= Bitboard(file, rank + dir);

		// Can perform double move? Only valid if we can move at least one step

		if (color == WHITE && rank == RANK_2) {
			if (!allPieces.contains(file, rank + 2)) {
				ret |= Bitboard(file, rank + 2);
			}
		}

		if (color == BLACK && rank == RANK_7) {
			if (!allPieces.contains(file, rank - 2)) {
				ret |= Bitboard(file, rank - 2);
			}
		}
	}

	// Can capture a left piece?
	if (file > FILE_A && oppPieces.contains(file - 1, rank + dir)) {
		ret |= Bitboard(file - 1, rank + dir);
	}

	// Can capture a right piece?
	if (file < FILE_H && oppPieces.contains(file + 1, rank + dir)) {
		ret |= Bitboard(file + 1, rank + dir);
	}

	return ret;
}

Bitboard MoveEval::getRookMoves(RankAndFile::Enum raf) const
{
	Bitboard ret;

	int rank = BoardUtils::getRank(raf);
	int file = BoardUtils::getFile(raf);

	// Right movement
	for (int i = 1; file + i <= FILE_H; ++i) {

		Bitboard dst(file + i, rank);
		ret |= dst;

		// Blocked by something?
		if (allPieces.overlaps(dst)) {
			break;
		}
	}

	// Left movement
	for (int i = 1; file - i >= FILE_A; ++i) {

		Bitboard dst(file - i, rank);
		ret |= dst;

		// Blocked by something?
		if (allPieces.overlaps(dst)) {
			break;
		}
	}

	// Upward movement
	for (int i = 1; rank + i <= RANK_8; ++i) {

		Bitboard dst(file, rank + i);
		ret |= dst;
		
		// Blocked by something?
		if (allPieces.overlaps(dst)) {
			break;
		}

	}

	// Downward movement
	for (int i = 1; rank - i >= RANK_1; ++i) {
		Bitboard dst(file, rank - i);
		ret |= dst;

		// Blocked by something?
		if (allPieces.overlaps(dst)) {
			break;
		}

	}

	// Exclude our own pieces.
	ret = ret & ~ownPieces;

	return ret;
}

Bitboard MoveEval::getKnightMoves(RankAndFile::Enum raf) const
{
	Bitboard ret;
	uint64_t pos = static_cast<uint64_t>(1) << static_cast<uint64_t>(raf);

	// Up 2, right 1
	ret |= Bitboard(pos << (8*2+1)) & ~Bitboard::file(FILE_A);
	// Up 2, left 1 
	ret |= Bitboard(pos << (8*2-1)) & ~Bitboard::file(FILE_H);
	// Down 2, right 1
	ret |= Bitboard(pos >> (8*2-1)) & ~Bitboard::file(FILE_A);
	// Down 2, left 1
	ret |= Bitboard(pos >> (8*2+1)) & ~Bitboard::file(FILE_H);

	// Right 2, up 1
	ret |= Bitboard(pos << (8*1+2)) & ~(Bitboard::file(FILE_A) | Bitboard::file(FILE_B));
	// Right 2, down 1
	ret |= Bitboard(pos >> (8*1-2)) & ~(Bitboard::file(FILE_A) | Bitboard::file(FILE_B));
	// Left 2, up 1
	ret |= Bitboard(pos << (8*1-2)) & ~(Bitboard::file(FILE_G) | Bitboard::file(FILE_H));
	// Left 2, down 1
	ret |= Bitboard(pos >> (8*1+2)) & ~(Bitboard::file(FILE_G) | Bitboard::file(FILE_H));

	// Exclude our own pieces.
	ret = ret & ~ownPieces;

	return ret;
}

Bitboard MoveEval::getBishopMoves(RankAndFile::Enum raf) const
{
	Bitboard ret;
	Bitboard tmp;

	uint64_t pos = static_cast<uint64_t>(1) << static_cast<uint64_t>(raf);

	// Up left diagonal
	tmp = Bitboard(pos);
	while (!tmp.empty()) {
		tmp = (tmp << (8-1)) & ~Bitboard::file(FILE_H);
		ret |= tmp;

		if (!(allPieces & tmp).empty())
			break;
	}

	// Up right diagonal
	tmp = Bitboard(pos);
	while (!tmp.empty()) {
		tmp = (tmp << (8+1)) & ~Bitboard::file(FILE_A);
		ret |= tmp;
		if (!(allPieces & tmp).empty())
			break;
	}

	// Down left diagonal
	tmp = Bitboard(pos);
	while (!tmp.empty()) {
		tmp = (tmp >> (8-1)) & ~Bitboard::file(FILE_A);
		ret |= tmp;
		if (!(allPieces & tmp).empty())
			break;
	}

	// Down right diagonal
	tmp = Bitboard(pos);
	while (!tmp.empty()) {
		tmp = (tmp >> (8+1)) & ~Bitboard::file(FILE_H);
		ret |= tmp;
		if (!(allPieces & tmp).empty())
			break;
	}

	// Exclude our own pieces.
	ret = ret & ~ownPieces;

	return ret;
}

Bitboard MoveEval::getQueenMoves(RankAndFile::Enum raf) const
{
	Bitboard ret;

	ret |= getBishopMoves(raf);
	ret |= getRookMoves(raf);

	return ret;
}

/// TODO: Castling
Bitboard MoveEval::getKingMoves(Color color, RankAndFile::Enum raf) const
{
	Bitboard ret;
	uint64_t pos = static_cast<uint64_t>(1) << static_cast<uint64_t>(raf);

	// Up
	ret |= Bitboard(pos << 8); // will truncate if we're at top rank
	// Down
	ret |= Bitboard(pos >> 8); // will truncate if we're at bottom rank
	// Left
	ret |= Bitboard(pos >> 1) & ~Bitboard::file(FILE_H);
	// Right
	ret |= Bitboard(pos << 1) & ~Bitboard::file(FILE_A);
	// Up Left
	ret |= Bitboard(pos << (8-1)) & ~Bitboard::file(FILE_H);
	// Up Right
	ret |= Bitboard(pos << (8+1)) & ~Bitboard::file(FILE_A);
	// Down Left
	ret |= Bitboard(pos >> (8+1)) & ~Bitboard::file(FILE_H);
	// Down Right
	ret |= Bitboard(pos >> (8-1)) & ~Bitboard::file(FILE_A);

	// Exclude our own pieces.
	ret = ret & ~ownPieces;

	return ret;
}

PositionEval::PositionEval(const Board &board_):
	board(board_)
{
}

int PositionEval::getScore() const
{
	return getScore(WHITE) - getScore(BLACK);
}

int PositionEval::getScore(Color color) const
{
	int ret = 0;

	// Sum raw piece values
	for (int i = 0; i <  64; ++i) {
		Square square = board.getSquare(static_cast<RankAndFile::Enum>(i));

		if (!square.isOccupied() || square.getColor() != color)
			continue;

		ret += getPieceValue(square.getPiece());
	}
	
	return ret;
}

int PositionEval::getPieceValue(Piece piece) const
{
	switch (piece) {
		case PAWN:   return 1000;
		case ROOK:   return 5000;
		case KNIGHT: return 3000;
		case BISHOP: return 3000;
		case QUEEN:  return 9000;
		case KING:   return 0; // Worth nothing, and everything.
	}

	assert(false && "Should be unreachable");
	return 0;
}

} // namespace vimlock
