#pragma once
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
	Move getMove() const;

	/// Return list of moves leading to this position
	MoveList getMoves() const;

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

	/// Squares occupied by any piece.
	Bitboard allPieces;

	/// Squares occupied by own pieces.
	Bitboard ownPieces;

	/// Squares occupied by opponent pieces.
	Bitboard oppPieces;

	/// Bitmask of our own king.
	Bitboard ownKing;

	int movesCount;
	Move moves[256];
};

enum MoveOrder
{
	MOVE_CAPTURE,
	MOVE_PROMOTE,
	MOVE_REGULAR
};

struct MoveCandidate
{
	MoveCandidate()
	{
	}

	MoveCandidate(Move move_, MoveOrder order_):
		move(move_),
		order(order_)
	{
	}

	Move move;
	MoveOrder order;
};

class Engine
{
public:
	Engine(int maxDepth=6);

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
	void traverse(Node *node, int alpha, int beta);

	/// Evaluate current nodes position, taking into account piece value, king safety, etc.
	void evaluate(Node *node);

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
