#pragma once
#include <vector>
#include <string>

#include "Square.h"

namespace vimlock
{

class Move
{
public:
	Move();
	Move(Square src, Square dst, Piece promote=PAWN);

	/// Return square where the piece was moved from.
	Square getSource() const;

	/// Return square where the piece is moved to.
	Square getDestination() const;

	/// Return piece to promote after this move, or PAWN if no promotion is applicable.
	Piece getPromotion() const;

	/// Returns true if this move promotes a piece to something.
	bool hasPromotion() const;

	/// Attempt to parse the move from long algebraic notation, e.g. "e2e4"
	/// If parsing failed, returns false.
	bool parseLan(const std::string &str);

	/// Return string representing this move in long algebraic notation, e.g. "e2e4"
	std::string toLan() const;

	bool operator == (Move rhs) const;
	bool operator != (Move rhs) const;

private:
	Square src;
	Square dst;
	Piece promote;
};

class MoveList : public std::vector<Move>
{
public:
	using std::vector<Move>::vector;

	/// Return string representing this move list in long algebraic notation, e.g. "e2e4 e7e5 f2f5"
	std::string toLan();
};

} // namespace vimlock
