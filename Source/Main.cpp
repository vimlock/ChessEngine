#include "Engine/Board.h"
#include "Engine/Format.h"
#include "Engine/Eval.h"

#include <iostream>

using namespace vimlock;
using namespace RankAndFile;

int main(int argc, const char *argv[])
{
	Board board;

	std::cout << toString(board) << std::endl;

	std::cout << std::endl;

	// board.setStandardPosition();
	board.setSquare(B2, WHITE, KING);

	MoveEval eval(board);
	Bitboard bb = eval.getAvailableMoves(B2);

	BoardTerminalFormatter fmt;
	fmt.setBoard(board);
	fmt.setBitboard(bb);

	std::cout << fmt.toString() << std::endl;

	std::cout << toString(bb);
}
