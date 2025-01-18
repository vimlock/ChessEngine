#include "Engine.h"
#include "Eval.h"
#include "Move.h"
#include "RankAndFile.h"
#include "Log.h"

#include <cassert>
#include <cstddef>
#include <limits>
#include <iostream>

namespace vimlock
{

using namespace RankAndFile;

Move Node::getMove() const
{
	return Move(src, dst, promote);
}

std::string Node::getPath() const
{
	std::string ret;

	const Node *it = this;
	while (it->parent != nullptr) {
		if (!ret.empty())
			ret += ' ';
		ret += it->getMove().toLan();
		it = it->parent;
	}

	return ret;
}

Engine::Engine()
{
	board.setStandardPosition();
}

void Engine::setPosition(const Board &board_)
{
	board = board_;
}

Board Engine::getPosition() const
{
	return board;
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

	if (!root->bestChild) {
		logError("No available move?");
		return false;
	}

	ret = root->bestChild->getMove();

	logInfo("Best move: " + ret.toLan()  + " " + std::to_string(root->bestChild->eval));

	return true;
}

void Engine::stop()
{
	// Nothing to do now as we're still single threaded
}

void Engine::traverse(Node *node)
{
	MoveEval eval(node->board);

	if (node->depth >= maxDepth) {
		evaluate(node, eval);
		return;
	}

	Bitboard ownPieces = eval.getOwnPieces();

	for (uint64_t i = 0; i < 64; ++i) {
		if (ownPieces & Bitboard(static_cast<uint64_t>(1)) << i)
			continue;

		Bitboard moves = eval.getAvailableMoves(static_cast<RankAndFile::Enum>(i));

		// Create a subnode for each possible move
		for (uint64_t k = 0; k < 64; ++k) {

			Bitboard dstBitboard = Bitboard(static_cast<uint64_t>(1) << k);

			// Not a valid move?
			if (!(moves & dstBitboard))
				continue;

			if (node->childCount >= Node::maxChildren)
				break;

			RankAndFile::Enum srcSquare = static_cast<RankAndFile::Enum>(i);
			RankAndFile::Enum dstSquare = static_cast<RankAndFile::Enum>(k);

			Piece piece = node->board.getSquare(srcSquare).getPiece();

			if (piece == PAWN && (dstBitboard & (Bitboard::rank(RANK_8) | Bitboard::rank(RANK_1)))) {
				// Add possible promotions
				addChildNode(node, srcSquare, dstSquare, ROOK);
				addChildNode(node, srcSquare, dstSquare, KNIGHT);
				addChildNode(node, srcSquare, dstSquare, BISHOP);
				addChildNode(node, srcSquare, dstSquare, QUEEN);
			}
			else {
				// Regular move
				addChildNode(node, srcSquare, dstSquare);
			}
		}
	}

	for (int i = 0; i < node->childCount; ++i) {
		traverse(node->children[i]);
	}

	minimax(node, board.getCurrent() != node->board.getCurrent());
}

void Engine::addChildNode(Node *parent, RankAndFile::Enum src, RankAndFile::Enum dst, Piece promote)
{
	Node *child = allocNode();
	child->src = src;
	child->dst = dst;
	child->board = parent->board;

	if (!child->board.movePiece(child->src, child->dst, child->promote)) {
		assert(false && "invalid move when traversing");
	}

	// Don't move into check
	if (MoveEval(child->board).isInCheck()) {
		freeNode(child);
		return;
	}

	child->board.flipCurrent();
	child->depth = parent->depth + 1;
	child->parent = parent;

	parent->children[parent->childCount++] = child;
}

void Engine::evaluate(Node *node, const MoveEval &eval)
{
	int own = getScore(node, eval, WHITE);
	int opp = getScore(node, eval, BLACK);

	node->eval = own - opp;

#if 0
	logInfo("eval " + node->getPath() + " " +
			std::to_string(own) + " - " + std::to_string(opp) +
			" = " + std::to_string(node->eval));
#endif
}

void Engine::minimax(Node *node, bool maximizing) const
{
	int eval;
	Node *best = nullptr;

	if (maximizing) {
		eval = std::numeric_limits<int>::min();
		for (int i = 0; i < node->childCount; ++i) {
			Node *child = node->children[i];
			if (eval < child->eval) {
				eval = child->eval;
				best = child;
			}
		}
	}
	else {
		eval = std::numeric_limits<int>::max();
		for (int i = 0; i < node->childCount; ++i) {
			Node *child = node->children[i];

			if (eval > child->eval) {
				eval = child->eval;
				best = child;
			}
		}
	}

	node->bestChild = best;
	node->eval = eval;
}

int Engine::getScore(Node *node, const MoveEval &eval, Color color) const
{
	int ret = 0;

	// Sum raw piece values
	for (int i = 0; i <  64; ++i) {
		SquareState square = node->board.getSquare(static_cast<RankAndFile::Enum>(i));

		if (!square.isOccupied() || square.getColor() != color)
			continue;

		ret += getPieceValue(square.getPiece());
	}

	if (eval.isInCheck()) {
		ret -= 500;
	}

	// Controlling more squares is good, I think?
	ret += eval.getAttackingSquares().count() * 10;

	// Even better if we're actually targetting something.
	ret += (eval.getOppPieces() & eval.getAttackingSquares()).count() * 10;

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
	node->depth = 0;
	node->promote = PAWN;
	node->eval = 0;
	node->bestChild = nullptr;
	node->childCount = 0;

	return node;
}

void Engine::freeNode(Node *node)
{
}

} // namespace vimlock
