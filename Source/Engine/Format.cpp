#include "Format.h"
#include "Bitboard.h"
#include "Board.h"

#include <cstring>

namespace vimlock
{

std::string toString(Color color, Piece piece)
{
	if (color == WHITE) {
		switch (piece) {
			case PAWN:   return "♙";
			case ROOK:   return "♖";
			case KNIGHT: return "♘";
			case BISHOP: return "♗";
			case QUEEN:  return "♕";
			case KING:   return "♔";
		}

		return "?";
	}
	else {
		switch (piece) {
			case PAWN:   return "♟";
			case ROOK:   return "♜";
			case KNIGHT: return "♞";
			case BISHOP: return "♝";
			case QUEEN:  return "♛";
			case KING:   return "♚";
		}

		return "?";
	}
}


BoardTerminalFormatter::BoardTerminalFormatter()
{
	std::memset(mask, 0x0, sizeof(mask));
}

void BoardTerminalFormatter::setBoard(const Board &board)
{
	for (int rank = RANK_1; rank <= RANK_8; rank++) {
		for (int file = FILE_A; file <= FILE_H; file++) {
			SquareState square = board.getSquare(file, rank);
			if (square.isOccupied())
				squares[rank][file] = ::vimlock::toString(square.getColor(), square.getPiece());
			else
				squares[rank][file] = ' ';
		}
	}
}

void BoardTerminalFormatter::setBitboard(const Bitboard &bitboard)
{
	for (int rank = 0; rank < 8; ++rank) {
		for (int file = 0; file < 8; ++file) {
			mask[rank][file] = bitboard.contains(file, rank);
		}
	}
}

std::string BoardTerminalFormatter::toString() const
{
	std::string ret;

	ret += " ABCDEFGH\n";

	for (int rank = RANK_8; rank >= RANK_1; --rank) {

		ret += std::to_string(rank + 1);

		for (int file = FILE_A; file <= FILE_H; ++file) {

			if (mask[rank][file]) {
				ret += "\033[42m";
			}

			ret += squares[rank][file];

			if (mask[rank][file]) {
				ret += "\033[0m";
			}
		}

		ret += "\n";
	}

	return ret;
}

std::string toString(const Bitboard &bitboard)
{
	std::string ret;

	for (int rank = RANK_8; rank >= RANK_1; --rank) {
		for (int file = FILE_A; file <= FILE_H; ++file) {
			ret += bitboard.contains(file, rank) ? "1" : "0";
		}
		ret += "\n";
	}

	return ret;
}

std::string toString(const Board &board)
{
	std::string ret;

	for (int rank = 7; rank >= 0; rank--) {

		ret += std::to_string(rank + 1);

		for (int file = 0; file < 8; file++) {
			SquareState square = board.getSquare(file, rank);
			if (square.isOccupied())
				ret += toString(square.getColor(), square.getPiece());
			else
				ret += " ";
		}

		ret += "\n";
	}

	ret += " ABCDEFGH";

	return ret;
}


} // namespace vimlock
