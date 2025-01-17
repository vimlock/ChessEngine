#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <string>

/// Various debugging print formatting functions

namespace vimlock
{

class Bitboard;
class Board;

/// Rendered view of the board state.
class BoardTerminalFormatter
{
public:
	BoardTerminalFormatter();

	/// Print the current board state to the buffer.
	void setBoard(const Board &board);

	/// Add bitboard for visualization.
	void setBitboard(const Bitboard &bitboard);

	/// Return string which can be outputted into a terminal
	std::string toString() const;

private:
	std::string squares[8][8];
	bool mask[8][8];
};

std::string toString(const Bitboard &bitboard);
std::string toString(const Board &board);

} // namespace vimlock

#endif // __FORMAT_H__
