#pragma once
#include "Move.h"
#include "Square.h"
#include "Enums.h"
#include "Bitboard.h"

namespace vimlock
{

/// Represents state of a single square on the board.
/// Square can be either unoccupied, or contain a piece+color.
class SquareState
{
public:
	/// Construct an empty square.
	SquareState();

	/// Construct a square with a piece occcupying it.
	SquareState(Color color, Piece piece);

	/// Returns true if this square has a piece.
	bool isOccupied() const;

	/// Returns color of the player who owns the occupying piece.
	Color getColor() const;

	/// Returns type of piece which occupies this square.
	Piece getPiece() const;

	/// Get raw bitwise representation.
	uint8_t getBits() const;

private:
	/// Bit layout is CCPPPPPP, where
	/// - C is the color of the owner
	/// - P is the type of the piece.
	///
	/// White pawn would be stored as  01000001
	/// Unoccupied square is stored with all bits set to 0.
	uint8_t bits;
};

/// Represents board state at a given point in time.
class Board
{
public:
	/// Construct an empty board.
	Board();

	/// Remove all pieces from board.
	void clear();
	
	/// Setup a classic initial board layout.
	void setStandardPosition();

	/// Assign a piece to given square.
	void setSquare(Square idx, Color color, Piece piece);
	void setSquare(RankAndFile idx, Color color, Piece piece);

	/// Assign given square state
	void setSquare(Square idx, SquareState square);

	/// Return state of given square
	SquareState getSquare(Square idx) const;

	/// Return state of given square
	SquareState getSquare(int file, int rank) const;

	/// Return current players turn.
	Color getCurrent() const;

	/// Set current players turn
	void setCurrent(Color color);

	/// Set current player from white to black and vice versa.
	void flipCurrent();

	/// Move given piece.
	/// Does not change current players turn.
	/// Return false if the move was not legal.
	bool movePiece(Square src, Square dst, Piece promote=PAWN);

	/// Apply given moves to board state.
	bool applyMoves(const MoveList &moves);

	/// Returns true if given square can be castled to
	/// 
	/// G1: white kingside castle
	/// C1: white queenside castle
	/// G8: black kingside castle
	/// C8: black queenside castle
	///
	/// NOTE: this does not check if the king is or would be in check
	/// NOTE: do not call with other squares
	bool canCastle(Square dst) const;

	/// Return bitboard of all non-empty squares.
	Bitboard getPieces() const;

	/// Return bitboard of all pieces with given color.
	Bitboard getPieces(Color color) const;

	/// Return bitboard of all squares which have given type of piece.
	Bitboard getPieces(Piece piece) const;

	/// Return bitboard of all squares with given color and type.
	Bitboard getPieces(Color color, Piece piece) const;

	/// Return moves available for given piece.
	///
	/// NOTE: usefull only for testing and debugging, does not cache any computed bitboards.
	Bitboard getMoves(Square idx) const;

private:
	Bitboard getPawnMoves(Color color, Square idx) const;
	Bitboard getPawnAttacks(Color color, Square idx) const;
	Bitboard getRookMoves(Square idx) const;
	Bitboard getKnightMoves(Square idx) const;
	Bitboard getBishopMoves(Square idx) const;
	Bitboard getQueenMoves(Square idx) const;
	Bitboard getKingMoves(Square idx) const;

	/// State of all the squares on the board.
	SquareState squares[64];

	/// Current players turn.
	Color current = WHITE;

	/// Bitboard of squares with castle rights.
	Bitboard castleRights;

	// TODO: en-passant state
};

} // namespace vimlock

#include "Board.inl"
