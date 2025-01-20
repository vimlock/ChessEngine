#include "Eval.h"
#include "Board.h"
#include "Moves.h"

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
	Color ownColor = color;
	Color oppColor = BoardUtils::getOpponent(color);

	ownPieces = board.getPieces(ownColor);
	oppPieces = board.getPieces(oppColor);
	allPieces = ownPieces | oppPieces;
	ownKing = board.getPieces(color, KING);

	// Cache squares where opponent is attacking.
	attackedSquares = getAvailableCaptures(board, allPieces, oppPieces);
	attackingSquares = getAvailableCaptures(board, allPieces, ownPieces);

	// Cache squares where we can move to
	for (uint64_t i = 0; i < 64; ++i) {
		if (ownPieces & (Bitboard(Square(i)))) {
			SquareState square = board.getSquare(i);
			ownAvailableMoves |= getAvailableMoves(ownColor, square.getPiece(), Square(i), allPieces, ownPieces);
		}
	}
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

} // namespace vimlock
