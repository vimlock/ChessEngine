#include "Engine.h"
#include "Uci.h"

#include <iostream>

using namespace vimlock;

int main(int argc, const char *argv[])
{
	Engine engine;

	Uci uci{engine};
	uci.main();

	return 0;
}
