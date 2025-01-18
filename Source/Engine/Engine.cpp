#include "Engine.h"
#include "Move.h"
#include "RankAndFile.h"

namespace vimlock
{

using namespace RankAndFile;

void Engine::setPosition(const Board &board)
{
}

void Engine::start()
{
}

bool Engine::poll(Move &ret)
{
	ret = Move(E2, E4);
	return true;
}

void Engine::stop()
{
}

} // namespace vimlock
