#ifndef __EVAL_H__
#define __EVAL_H__

#include "Bitboard.h"
#include "Board.h"
#include "RankAndFile.h"

namespace vimlock
{

class Board;

class BoardUtils
{
public:
	/// Return bitboard of all pieces for given color.
	static Bitboard getPieces(const Board &board, Color color);

	/// Return bitboard of given type of pieces.
	static Bitboard getPieces(const Board &board, Piece piece);

	/// Return bitboard of given type of pieces.
	static Bitboard getPieces(const Board &board, Color color, Piece piece);

	/// Get rank from RankAndFile
	static int getRank(RankAndFile::Enum raf);

	/// Get file from RankAndFile
	static int getFile(RankAndFile::Enum raf);
};

/// Evaluate board state for available moves
class MoveEval
{
public:
	MoveEval(const Board &board);

	/// Returns bitboard of all positions given piece can be moved to
	Bitboard getAvailableMoves(RankAndFile::Enum raf) const;

	/// Return bitboard of positions which given piece can attack to
	/// Almost same as `getAvailableCaptures()` but differs in case of pawns
	/// which can capture only diagonally.
	Bitboard getAvailableCaptures(RankAndFile::Enum raf) const;

	/// Return squares which opponent is currently attacking (and where king can't be moved to).
	Bitboard getAttackedSquares() const;

	/// Returns true if the current player is currently is check.
	bool isInCheck() const;

private:
	Bitboard getPawnMoves(Color color, RankAndFile::Enum raf) const;
	Bitboard getPawnAttacks(Color color, RankAndFile::Enum raf) const;
	Bitboard getRookMoves(RankAndFile::Enum raf) const;
	Bitboard getKnightMoves(RankAndFile::Enum raf) const;
	Bitboard getBishopMoves(RankAndFile::Enum raf) const;
	Bitboard getQueenMoves(RankAndFile::Enum raf) const;
	Bitboard getKingMoves(Color color, RankAndFile::Enum raf) const;

	/// Return color of the opposing player.
	Color getOpponent(Color color) const;

	/// Current board state.
	const Board &board;

	/// Squares occupied by any piece.
	Bitboard allPieces;

	/// Squares occupied by opponent pieces.
	Bitboard oppPieces;

	/// Squares occupied by own pieces.
	Bitboard ownPieces;

	/// Bitmask of our own king.
	Bitboard ownKing;

	/// Squares currently threathened by opponent.
	Bitboard attackedSquares;
};

/// Evaluate current position, taking into account piece value, king safety, etc.
class PositionEval
{
public:
	PositionEval(const Board &board);

	int getScore() const;

private:
	int getScore(Color color) const;

	int getPieceValue(Piece piece) const;

	const Board &board;
};

} // namespace vimlock

#endif // __EVAL_H__
