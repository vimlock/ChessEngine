#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Board.h"

namespace vimlock
{

class Move;

struct Node
{
	/// Board state at this node
	Board board;

	/// Piece which was moved.
	RankAndFile::Enum src;

	/// Move which led to this node
	RankAndFile::Enum dst;

	/// Evaluation at this point.
	int eval;

	/// Evaluation depth at this node
	int depth;

	Node *parent;

	Node *nextSibling;

	Node *firstChild;
};

class Engine
{
public:
	Engine();

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
	void traverse(Node *node);

	/// Evaluate current nodes position, taking into account piece value, king safety, etc.
	void evaluate(Node *node);

	int getScore(Node *node, Color color) const;

	Node * allocNode();
	void freeNode(Node *node);

	int getPieceValue(Piece piece) const;

	Board board;
	
	int maxDepth = 2;
};

} // namespace vimlock

#endif // ___ENGINE_H__
