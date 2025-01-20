#pragma once
#include "Bitboard.h"
#include "Board.h"

namespace vimlock
{

class Board;

class BoardUtils
{
public:
	/// Return color of the opposing player.
	static Color getOpponent(Color color);
};

/// Evaluate board state for available moves
class MoveEval
{
public:
	MoveEval(const Board &board, Color color);

	/// Return bitboard of all of our own pieces.
	Bitboard getOwnPieces() const;

	/// Return bitboard of opponent pieces.
	Bitboard getOppPieces() const;

	/// Return squares which opponent is currently attacking (and where king can't be moved to).
	Bitboard getAttackedSquares() const;

	/// Return squares where we are currently attacking.
	Bitboard getAttackingSquares() const;

	/// Return bitboard of every square where we can move to.
	Bitboard getOwnAllAvailableMoves() const;

	/// Returns true if the current player is currently is check.
	bool isInCheck() const;

private:
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

	/// Squares currently threathened by us.
	Bitboard attackingSquares;

	/// All squares where we can move to.
	Bitboard ownAvailableMoves;
};

} // namespace vimlock
