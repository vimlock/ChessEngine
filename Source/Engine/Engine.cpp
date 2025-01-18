#include "Engine.h"
#include "Eval.h"
#include "Move.h"
#include "RankAndFile.h"
#include "Log.h"

#include <cassert>
#include <cstddef>

namespace vimlock
{

using namespace RankAndFile;

Engine::Engine()
{
	board.setStandardPosition();
}

void Engine::setPosition(const Board &board_)
{
	board = board_;
}

void Engine::start()
{
	// Nothing to do now as we're still single threaded
}

bool Engine::poll(Move &ret)
{
	Node *root = allocNode();
	root->depth = 0;
	root->board = board;

	traverse(root);

	if (!root->firstChild) {
		logError("No available moves?");
		return false;
	}

	ret = Move(root->firstChild->src, root->firstChild->dst);

	return true;
}

void Engine::stop()
{
	// Nothing to do now as we're still single threaded
}

void Engine::traverse(Node *node)
{
	evaluate(node);

	if (node->depth >= maxDepth) {
		logInfo("Max depth reached");
		return;
	}

	MoveEval eval(node->board);

	const Bitboard &ownPieces = eval.getOwnPieces();

	for (uint64_t i = 0; i < 64; ++i) {
		if ((ownPieces & Bitboard(static_cast<uint64_t>(1)) << i).empty())
			continue;

		Bitboard moves = eval.getAvailableMoves(static_cast<RankAndFile::Enum>(i));

		// Create a subnode for each possible move
		for (uint64_t k = 0; k < 64; ++k) {
			if ((moves & Bitboard(static_cast<uint64_t>(1) << k)).empty())
				continue;

			logInfo("Add node " + std::to_string(k));

			Node *child = allocNode();
			child->board = node->board;
			child->depth = node->depth + 1;
			child->board.flipCurrent();
			child->parent = node;
			child->src = static_cast<RankAndFile::Enum>(i);
			child->dst = static_cast<RankAndFile::Enum>(k);

			child->nextSibling = node->firstChild;
			node->firstChild = child;
		}
	}
}

void Engine::evaluate(Node *node)
{
	node->eval = getScore(node, WHITE) - getScore(node, BLACK);
}

int Engine::getScore(Node *node, Color color) const
{
	int ret = 0;

	// Sum raw piece values
	for (int i = 0; i <  64; ++i) {
		Square square = board.getSquare(static_cast<RankAndFile::Enum>(i));

		if (!square.isOccupied() || square.getColor() != color)
			continue;

		ret += getPieceValue(square.getPiece());
	}
	
	return ret;
}

int Engine::getPieceValue(Piece piece) const
{
	switch (piece) {
		case PAWN:   return 1000;
		case ROOK:   return 5000;
		case KNIGHT: return 3000;
		case BISHOP: return 3000;
		case QUEEN:  return 9000;
		case KING:   return 0; // Worth nothing, yet everything.
	}

	assert(false && "Should be unreachable");
	return 0;
}

Node * Engine::allocNode()
{
	Node *node = new Node();

	node->parent = nullptr;
	node->nextSibling = nullptr;
	node->firstChild = nullptr;
	node->depth = 0;
	node->eval = 0;

	return node;
}

void Engine::freeNode(Node *node)
{
}

} // namespace vimlock
