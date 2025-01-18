#ifndef __ENGINE_H__
#define __ENGINE_H__

namespace vimlock
{

class Move;
class Board;

class Engine
{
public:
	/// Set current board position.
	void setPosition(const Board &board);

	/// Start searching for a best move from current position.
	void start();

	/// If a best move is available, stores it in `ret` and returns true.
	/// If best move is not yet available, returns false and leaves `ret` untouched.
	bool poll(Move &ret);

	/// Stop searching for the best move.
	void stop();
private:
};

} // namespace vimlock

#endif // ___ENGINE_H__
