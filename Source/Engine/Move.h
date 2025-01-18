#ifndef __MOVE_H__
#define __MOVE_H__

#include <vector>
#include <string>

#include "Engine/RankAndFile.h"

namespace vimlock
{

struct Move
{
public:
	Move();
	Move(RankAndFile::Enum src, RankAndFile::Enum dst);

	/// Return square where the piece was moved from.
	RankAndFile::Enum getSource() const;

	/// Return square where the piece is moved to.
	RankAndFile::Enum getDestination() const;

	/// Attempt to parse the move from long algebraic notation, e.g. "e2e4"
	/// If parsing failed, returns false.
	bool parseLan(const std::string &str);

	/// Return string reprenting this move in long algebraic notation, e.g. "e2e4"
	std::string toLan() const;

	bool operator == (Move rhs) const;
	bool operator != (Move rhs) const;

private:
	RankAndFile::Enum src;
	RankAndFile::Enum dst;
};

} // namespace vimlock

#endif // __MOVE_H__
