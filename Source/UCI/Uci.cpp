#include "Uci.h"
#include "Engine/Board.h"
#include "Engine/Engine.h"
#include "Engine/Move.h"
#include "Engine/Log.h"
#include "Engine/Format.h"

#include <iostream>
#include <sstream>
#include <list>

namespace vimlock
{

static bool startswith(const std::string &str, const std::string &prefix)
{
	return str.rfind(prefix, 0) == 0;
}

Uci::Uci(Engine &engine):
	Uci(engine, std::cin, std::cout)
{
}

Uci::Uci(Engine &engine_, std::istream &input_, std::ostream &output_):
	engine(engine_),
	input(input_),
	output(output_)
{
}

void Uci::main()
{
	std::string line;

	quit = false;

	while (std::getline(input, line)) {
		onLine(line);
		if (quit)
			break;
	}
}

void Uci::onLine(const std::string &line)
{
	logInfo("UCI <-- " + line);

	if (startswith(line, "go"))
		onGo(line);
	else if (line == "isready")
		onIsReady(line);
	else if (startswith(line, "position "))
		onPosition(line);
	else if (line == "quit")
		onQuit(line);
	else if (line == "stop")
		onQuit(line);
	else if (line == "uci")
		onUci(line);
	else if (line == "ucinewgame")
		onUciNewGame(line);
	else
		onUnknown(line);
}

void Uci::send(const std::string &line)
{
	output << line << std::endl;
	output.flush();

	logInfo("UCI --> " + line);
}

void Uci::onIsReady(const std::string &line)
{
	send("readyok");
}

void Uci::onGo(const std::string &line)
{
	Move move;

	Board board = engine.getPosition();
	std::cerr << toString(board) << std::endl;

	if (engine.poll(move)) {
		send("bestmove " + move.toLan());
	}

	board.applyMoves({move});
	std::cerr << toString(board) << std::endl;
}

void Uci::onUciNewGame(const std::string &line)
{
}

/// TODO: Handle FEN definitions
void Uci::onPosition(const std::string &line)
{
	Board board;

	std::list<std::string> parts;
	std::istringstream stream(line);
	std::string part;
	while (std::getline(stream, part, ' '))
		parts.push_back(part);

	// Remove "position"
	parts.pop_front();

	if (parts.empty()) {
		logError("Invalid command: " + line);
		return;
	}

	if (parts.front() == "startpos") {
		board.setStandardPosition();
		parts.pop_front();
	}
	else if (parts.front() == "fen") {
		logError("FEN strings not implemented");
		parts.pop_front();
	}

	if (!parts.empty()) {
		if (parts.front() == "moves") {
			MoveList moves;
			parts.pop_front();

			for (const std::string &it : parts) {
				Move move;
				if (!move.parseLan(it)) {
					logError("Invalid command: " + line);
				}

				moves.push_back(move);
			}

			if (!board.applyMoves(moves)) {
				logError("Invalid moves on position: " + line);
			}
		}
		else {
			logError("Invalid command: " + line);
			return;
		}
	}

	engine.setPosition(board);
}

void Uci::onQuit(const std::string &line)
{
	quit = true;
}

void Uci::onStop(const std::string &line)
{
	engine.stop();
}

void Uci::onUci(const std::string &line)
{
	send("id name EngineDemo");
	send("id author Joel Polso");
	send("uciok");
}

void Uci::onUnknown(const std::string &line)
{
	logError("Unknown command: " + line);
}

} // namespace vimlock
