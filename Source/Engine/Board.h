#ifndef __BOARD_H__
#define __BOARD_H__

#include "Move.h"

namespace vimlock
{

/// Represents side of the player
enum Color
{
	WHITE = 64,
	BLACK = 128,
};

enum
{
	COLOR_MASK = WHITE | BLACK
};

/// Represents state of a single square on the board.
/// Square can be either unoccupied, or contain a piece+color.
class Square
{
public:
	/// Construct an empty square.
	Square();

	/// Construct a square with a piece occcupying it.
	Square(Color color, Piece piece);

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
	void setSquare(RankAndFile::Enum raf, Color color, Piece piece);

	/// Assign given square state
	void setSquare(RankAndFile::Enum raf, Square square);

	/// Return state of given square
	Square getSquare(RankAndFile::Enum raf) const;

	/// Return state of given square
	Square getSquare(int file, int rank) const;

	/// Return current players turn.
	Color getCurrent() const;

	/// Set current players turn
	void setCurrent(Color color);

	/// Set current player from white to black and vice versa.
	void flipCurrent();

	/// Move given piece.
	/// Does not change current players turn.
	/// Return false if the move was not legal.
	bool movePiece(RankAndFile::Enum src, RankAndFile::Enum dst, Piece promote=PAWN);

	/// Apply given moves to board state.
	bool applyMoves(const MoveList &moves);

private:

	/// State of all the squares on the board.
	Square squares[64];

	/// Current players turn.
	Color current = WHITE;

	/// Bitmask of players who can still castle.
	unsigned castleRights = WHITE | BLACK;

	// TODO: en-passant state
};

} // namespace vimlock

#endif // __BOARD_H__
