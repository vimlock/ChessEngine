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

private:
	Bitboard getPawnMoves(Color color, RankAndFile::Enum raf) const;
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
