#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Board.h"

namespace vimlock
{

class Move;
class MoveEval;

struct Evaluation
{
	int eval;
	Move best;
	MoveList continuation;
};

struct Node
{
	/// Theoretical maximum is 218 based on web search.
	static const int maxChildren = 256;

	Move getMove() const;

	/// Return list of moves leading to this position
	MoveList getPath() const;

	/// Board state at this node
	Board board;

	/// Piece which was moved.
	Square src;

	/// Move which led to this node
	Square dst;

	/// If the move promoted to something.
	Piece promote;

	/// Evaluation at this point.
	int eval;

	/// Evaluation depth at this node
	int depth;

	Node *parent;

	Node *bestChild;

	int childCount;
	Node *children[maxChildren];
};

class Engine
{
public:
	Engine();

	/// Set current board position.
	void setPosition(const Board &board);

	/// Get current board position.
	Board getPosition() const;

	/// Start searching for a best move from current position.
	void start();

	/// If a best move is available, stores it in `ret` and returns true.
	/// If best move is not yet available, returns false and leaves `ret` untouched.
	bool poll(Evaluation &ret);

	/// Stop searching for the best move.
	void stop();

private:
	void traverse(Node *node);

	void addChildNode(Node *parent, Square src, Square dst, Piece promote=PAWN);

	/// Evaluate current nodes position, taking into account piece value, king safety, etc.
	void evaluate(Node *node);

	void minimax(Node *node, bool maximizing) const;

	int getScore(const Board &board, Color color) const;

	Node * allocNode();
	void freeNode(Node *node);

	int getPieceValue(Piece piece) const;

	Board board;
	
	int maxDepth = 2;
};

} // namespace vimlock

#endif // ___ENGINE_H__
