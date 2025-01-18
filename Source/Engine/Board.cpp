#include "Board.h"
#include "Eval.h"
#include "Move.h"
#include "Log.h"

#include <cassert>

namespace vimlock
{

Square::Square():
	bits(0)
{
}

Square::Square(Color color, Piece piece):
	bits(color | piece)
{
}

bool Square::isOccupied() const
{
	return bits & PIECE_MASK;
}

Color Square::getColor() const
{
	assert(isOccupied());
	return static_cast<Color>(bits & COLOR_MASK);
}

Piece Square::getPiece() const
{
	assert(isOccupied());
	return static_cast<Piece>(bits & PIECE_MASK);
}

uint8_t Square::getBits() const
{
	return bits;
}

Board::Board()
{
}

void Board::clear()
{
	for (int i = 0; i < 64; ++i)
		squares[i] = Square();
}

void Board::setStandardPosition()
{
	using namespace RankAndFile;

	// Clear any previous board state.
	clear();

	// White side setup.
	setSquare(A1, WHITE, ROOK);
	setSquare(B1, WHITE, KNIGHT);
	setSquare(C1, WHITE, BISHOP);
	setSquare(D1, WHITE, QUEEN);
	setSquare(E1, WHITE, KING);
	setSquare(F1, WHITE, BISHOP);
	setSquare(G1, WHITE, KNIGHT);
	setSquare(H1, WHITE, ROOK);

	setSquare(A2, WHITE, PAWN);
	setSquare(B2, WHITE, PAWN);
	setSquare(C2, WHITE, PAWN);
	setSquare(D2, WHITE, PAWN);
	setSquare(E2, WHITE, PAWN);
	setSquare(F2, WHITE, PAWN);
	setSquare(G2, WHITE, PAWN);
	setSquare(H2, WHITE, PAWN);

	// Black side setup.
	setSquare(A8, BLACK, ROOK);
	setSquare(B8, BLACK, KNIGHT);
	setSquare(C8, BLACK, BISHOP);
	setSquare(D8, BLACK, QUEEN);
	setSquare(E8, BLACK, KING);
	setSquare(F8, BLACK, BISHOP);
	setSquare(G8, BLACK, KNIGHT);
	setSquare(H8, BLACK, ROOK);

	setSquare(A7, BLACK, PAWN);
	setSquare(B7, BLACK, PAWN);
	setSquare(C7, BLACK, PAWN);
	setSquare(D7, BLACK, PAWN);
	setSquare(E7, BLACK, PAWN);
	setSquare(F7, BLACK, PAWN);
	setSquare(G7, BLACK, PAWN);
	setSquare(H7, BLACK, PAWN);
}

void Board::setSquare(RankAndFile::Enum raf, Color color, Piece piece)
{
	setSquare(raf, Square(color, piece));
}

void Board::setSquare(RankAndFile::Enum raf, Square square)
{
	squares[raf] = square;
}

Square Board::getSquare(RankAndFile::Enum raf) const
{
	return squares[raf];
}

Square Board::getSquare(int file, int rank) const
{
	return getSquare(static_cast<RankAndFile::Enum>(rank * 8 + file));
}

Color Board::getCurrent() const
{
	return current;
}

void Board::setCurrent(Color color)
{
	current = color;
}

void Board::flipCurrent()
{
	setCurrent(BoardUtils::getOpponent(current));
}

bool Board::applyMoves(const MoveList &moves)
{
	for (const Move &it : moves) {

		if (!movePiece(it.getSource(), it.getDestination()))
			return false;

		flipCurrent();
	}

	return true;
}

/// TODO: Currently this allows capturing our own pieces
bool Board::movePiece(RankAndFile::Enum src, RankAndFile::Enum dst)
{
	Square tmp = getSquare(src);
	if (!tmp.isOccupied()) {
		logError("Source square is not occupied");
		return false;
	}

	setSquare(dst, tmp);
	setSquare(src, Square());

	return true;
}

} // namespace vimlock
