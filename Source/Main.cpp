#include "Engine/Engine.h"
#include "UCI/Uci.h"

#include <iostream>

using namespace vimlock;

int main(int argc, const char *argv[])
{
	// BoardTerminalFormatter fmt;
	// fmt.setBoard(board);
	// fmt.setBitboard(bb);
	
	Engine engine;

	Uci uci{engine};
	uci.main();

	return 0;
}
