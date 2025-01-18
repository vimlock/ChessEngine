#ifndef __UCI_H__
#define __UCI_H__

#include <iosfwd>
#include <string>

namespace vimlock
{

class Engine;

class Uci
{
public:
	Uci(Engine &engine);
	Uci(Engine &engine, std::istream &input, std::ostream &output);

	void main();

private:
	void onLine(const std::string &line);
	void onGo(const std::string &line);
	void onIsReady(const std::string &line);
	void onPosition(const std::string &line);
	void onQuit(const std::string &line);
	void onStop(const std::string &line);
	void onUci(const std::string &line);
	void onUciNewGame(const std::string &line);
	void onUnknown(const std::string &line);

	void send(const std::string &line);

	Engine &engine;
	std::istream &input;
	std::ostream &output;

	bool quit = false;
};

} // namespace vimlock

#endif // __UCI_H__
