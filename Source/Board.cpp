#include "Board.h"
#include "Moves.h"
#include "Eval.h"
#include "Format.h"
#include "Log.h"

namespace vimlock
{

// Starting squares of pawns.
constexpr Bitboard pawnRanks = Bitboard::rank(RANK_7) | Bitboard::rank(RANK_2);

// Squares where pawn would land after double move
constexpr Bitboard pawnDoubleMoveRanks = Bitboard::rank(RANK_5) | Bitboard::rank(RANK_4);

Board::Board()
{
	castleRights = Bitboard(A1)
		| Bitboard(E1)
		| Bitboard(H1)
		| Bitboard(A8)
		| Bitboard(E8)
		| Bitboard(H8);
}

void Board::clear()
{
	for (int i = 0; i < 64; ++i)
		squares[i] = SquareState();
}

void Board::setStandardPosition()
{
	castleRights = Bitboard(A1)
		| Bitboard(E1)
		| Bitboard(H1)
		| Bitboard(A8)
		| Bitboard(E8)
		| Bitboard(H8);

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


void Board::flipCurrent()
{
	setCurrent(BoardUtils::getOpponent(current));
}

bool Board::applyMoves(const MoveList &moves)
{
	for (const auto &it : moves) {

		if (!movePiece(it.getSource(), it.getDestination(), it.getPromotion()))
			return false;

		flipCurrent();
	}

	return true;
}

/// TODO: Currently this allows capturing our own pieces if caller so wants
bool Board::movePiece(Square src, Square dst, Piece promote)
{
	SquareState tmp = getSquare(src);
	if (!tmp.isOccupied()) {
		logError("Source square is not occupied");
		return false;
	}

	if (promote != PAWN) {
		tmp = SquareState(tmp.getColor(), promote);
	}

	setSquare(dst, tmp);
	setSquare(src, SquareState());

	// Remove any castle rights the piece had
	castleRights = castleRights & ~Bitboard(src);

	if (tmp.getPiece() == KING) {
		if (dst.getFile() == src.getFile() + 2) {
			// Castling kingside
			movePiece(Square(FILE_H, dst.getRank()), Square(FILE_F, dst.getRank()));
		}
		else if (dst.getFile() == src.getFile() - 2) {
			// Castling queenside
			movePiece(Square(FILE_A, dst.getRank()), Square(FILE_D, dst.getRank()));
		}
	}

	// Was this an en passant move?
	if (tmp.getPiece() == PAWN && (Bitboard(dst) & enpassantSquares)) {

		Square enpassanted = Square(dst.getFile(), src.getRank());

		assert(getSquare(enpassanted).isOccupied());
		// Capture the en-passanted square
		setSquare(enpassanted, SquareState());
	}

	// Clear any pawns from being en-passanted from previous turn.
	enpassantSquares = Bitboard();

	// Pawn can be en-passanted by opponent?
	if (tmp.getPiece() == PAWN && (Bitboard(src) & pawnRanks) && (Bitboard(dst) & pawnDoubleMoveRanks)) {
		if (dst.getRank() == RANK_4)
			enpassantSquares = Bitboard(Square(src.getFile(), RANK_3));
		else
			enpassantSquares = Bitboard(Square(src.getFile(), RANK_6));

	}

	return true;
}

bool Board::canCastle(Square dst) const
{
	Bitboard whitek = Bitboard(H1) | Bitboard(E1);
	Bitboard whiteq = Bitboard(A1) | Bitboard(E1);
	Bitboard blackk = Bitboard(H8) | Bitboard(E8);
	Bitboard blackq = Bitboard(A8) | Bitboard(E8);

	switch (dst.getIndex()) {
		case G1: return (castleRights & whitek) == whitek;
		case C1: return (castleRights & whiteq) == whiteq;
		case G8: return (castleRights & blackk) == blackk;
		case C8: return (castleRights & blackq) == blackq;
	}

	assert(false && "Should be unreachable");
	return false;
}

Bitboard Board::getMoves(Square idx) const
{
	Bitboard allPieces = getPieces();
	SquareState square = getSquare(idx);
	Bitboard ownPieces = getPieces(square.getColor());

	return getAvailableMoves(square.getColor(), square.getPiece(), idx, allPieces, ownPieces);
}

} // namespace vimlock
