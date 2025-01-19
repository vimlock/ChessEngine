#ifndef __EVAL_H__
#define __EVAL_H__

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

	/// Returns bitboard of all positions given piece can be moved to
	///
	/// NOTE: none of these functions test for check or castling, that is handled
	/// when searching through available nodes
	Bitboard getAvailableMoves(Square idx) const;

	/// Return bitboard of positions which given piece can attack to
	/// Almost same as `getAvailableCaptures()` but differs in case of pawns
	/// which can capture only diagonally.
	Bitboard getAvailableCaptures(Square idx) const;

	/// Return squares which opponent is currently attacking (and where king can't be moved to).
	Bitboard getAttackedSquares() const;

	/// Return squares where we are currently attacking.
	Bitboard getAttackingSquares() const;

	/// Return bitboard of every square where we can move to.
	Bitboard getOwnAllAvailableMoves() const;

	/// Returns true if the current player is currently is check.
	bool isInCheck() const;

private:
	/// Return bitboard of all pieces for given color.
	Bitboard getPieces(Color color) const;

	/// Return bitboard of given type of pieces.
	Bitboard getPieces(Piece piece) const;

	/// Return bitboard of given type of pieces.
	Bitboard getPieces(Color color, Piece piece) const;

	Bitboard getPawnMoves(Color color, Square idx) const;
	Bitboard getPawnAttacks(Color color, Square idx) const;
	Bitboard getRookMoves(Square idx) const;
	Bitboard getKnightMoves(Square idx) const;
	Bitboard getBishopMoves(Square idx) const;
	Bitboard getQueenMoves(Square idx) const;
	Bitboard getKingMoves(Square idx) const;

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

#endif // __EVAL_H__
