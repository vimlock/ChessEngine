#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Board.h"

namespace vimlock
{

class Move;
class MoveEval;

struct Evaluation
{
	/// How strong engine considers the current position if
	/// going by the continuation.
	int eval;

	/// The next best move.
	Move best;

	/// Best contination according to the engine.
	MoveList continuation;

	/// Total number of positions evaluated.
	uint64_t total;
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

	/// Set if the move leading to this position was a promotion.
	Piece promote;

	/// Evaluation at this point.
	int eval;

	/// Evaluation depth at this node
	int depth;

	/// Expensive to calculate to do it once and cache the result here.
	Bitboard allPieces;
	Bitboard ownPieces;
	Bitboard oppPieces;

	Node *parent;

	Node *bestChild;

	int childCount;
	Node *children[maxChildren];
};

class Engine
{
public:
	Engine(int maxDepth=5);

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
	
	int maxDepth;

	/// Statistics
	uint64_t total = 0;
};

} // namespace vimlock

#endif // ___ENGINE_H__
