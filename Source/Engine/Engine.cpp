#include "Engine.h"
#include "Eval.h"
#include "Move.h"
#include "Moves.h"
#include "Log.h"

#include <cassert>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <iostream>

namespace vimlock
{

// 4x4 region in center
static Bitboard centerSquares = Bitboard(0x3c3c000000LL);

// A and H file
static Bitboard edges = Bitboard(0x20c0c18181818181);

static Bitboard topAndBottomRanks = Bitboard::rank(RANK_8) | Bitboard::rank(RANK_1);

Move Node::getMove() const
{
	return Move(src, dst, promote);
}

MoveList Node::getPath() const
{
	MoveList ret;

	const Node *it = this;
	while (it->parent != nullptr) {
		ret.push_back(getMove());
		it = it->parent;
	}

	std::reverse(ret.begin(), ret.end());

	return ret;
}

Engine::Engine(int maxDepth_):
	maxDepth(maxDepth_)
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

bool Engine::poll(Evaluation &ret)
{
	Node *root = allocNode();
	root->depth = 0;
	root->board = board;
	root->allPieces = root->board.getPieces();
	root->ownPieces = root->board.getPieces(root->board.getCurrent());
	root->oppPieces = root->board.getPieces(BoardUtils::getOpponent(root->board.getCurrent()));

	total = 0;

	traverse(root);

	if (!root->bestChild) {
		logError("No available move?");
		freeNode(root);
		return false;
	}

	ret.best = root->bestChild->getMove();
	ret.eval = root->bestChild->eval;
	ret.total = total;

	for (Node *it = root->bestChild; it != nullptr; it = it->bestChild) {
		ret.continuation.push_back(it->getMove());
	}

	freeNode(root);

	return true;
}

void Engine::stop()
{
	// Nothing to do now as we're still single threaded
}

void Engine::traverse(Node *node)
{
	if (node->depth >= maxDepth) {
		evaluate(node);
		return;
	}

	Bitboard allPieces = node->allPieces;
	Bitboard ownPieces = node->ownPieces;

	for (uint64_t i = 0; i < 64; ++i) {
		if (!(ownPieces & Bitboard(Square(i))))
			continue;

		SquareState square = node->board.getSquare(Square(i));

		Bitboard moves = getAvailableMoves(
			square.getColor(), square.getPiece(), Square(i), allPieces, ownPieces);

		// Create a subnode for each possible move
		for (uint64_t k = 0; k < 64; ++k) {

			Bitboard dstBitboard = Bitboard(Square(k));

			// Not a valid move?
			if (!(moves & dstBitboard))
				continue;

			if (node->childCount >= Node::maxChildren)
				break;

			Square srcSquare = Square(i);
			Square dstSquare = Square(k);

			if (square.getPiece() == PAWN && (dstBitboard & (topAndBottomRanks))) {
				// Add possible promotions
				addChildNode(node, srcSquare, dstSquare, ROOK);
				addChildNode(node, srcSquare, dstSquare, KNIGHT);
				addChildNode(node, srcSquare, dstSquare, BISHOP);
				addChildNode(node, srcSquare, dstSquare, QUEEN);
			}
			else {
				// Regular move
				addChildNode(node, srcSquare, dstSquare);

				if (square.getPiece() == KING) {
					// TODO: consider castling
				}
			}
		}
	}

	// In a terminal node? Due to a checkmate, or stalemate
	if (node->childCount == 0) {
		evaluate(node);
		return;
	}

	for (int i = 0; i < node->childCount; ++i) {
		traverse(node->children[i]);
	}

	minimax(node, board.getCurrent() == node->board.getCurrent());

	// Free all but the best candidate
	for (int i = 0; i < node->childCount; ++i) {
		if (node->children[i] != node->bestChild) {
			freeNode(node->children[i]);
		}
	}
	node->childCount = 0;
}

void Engine::addChildNode(Node *parent, Square src, Square dst, Piece promote)
{
	Node *child = allocNode();
	child->src = src;
	child->dst = dst;
	child->board = parent->board;
	child->promote = promote;

	if (!child->board.movePiece(child->src, child->dst, child->promote)) {
		assert(false && "invalid move when traversing");
	}

	Bitboard allPieces = child->board.getPieces();
	Bitboard ownPieces = child->board.getPieces(child->board.getCurrent());
	Bitboard oppPieces = allPieces & ~ownPieces;
	Bitboard ownKing = child->board.getPieces(child->board.getCurrent(), KING);
	Bitboard oppAttacks = getAvailableCaptures(child->board, allPieces, oppPieces);

	// Don't move into check
	if (oppAttacks & ownKing) {
		freeNode(child);
		return;
	}

	child->board.flipCurrent();
	child->depth = parent->depth + 1;
	child->parent = parent;

	// Flipped around as the turn changed
	child->allPieces = allPieces;
	child->ownPieces = oppPieces;
	child->oppPieces = ownPieces;

	parent->children[parent->childCount++] = child;
}

void Engine::evaluate(Node *node)
{
	int own = getScore(node->board, board.getCurrent());
	int opp = getScore(node->board, BoardUtils::getOpponent(board.getCurrent()));

	node->eval = own - opp;

#if 0
	logInfo("eval " + node->getPath().toLan() + " " +
			std::to_string(own) + " - " + std::to_string(opp) +
			" = " + std::to_string(node->eval),
		node->board
	);
#endif

	total++;
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

int Engine::getScore(const Board &board, Color color) const
{
	MoveEval eval(board, color);

	int ret = 0;

	// Sum raw piece values
	for (uint64_t i = 0; i <  64; ++i) {
		SquareState square = board.getSquare(Square(i));

		if (!square.isOccupied() || square.getColor() != color)
			continue;

		ret += getPieceValue(square.getPiece());
	}

	if (eval.isInCheck()) {
		ret -= 500;
	}

	// Piece mobility is good.
	ret += eval.getOwnAllAvailableMoves().count() * 100;

	// Even better if we're actually targetting something.
	ret += (eval.getOppPieces() & eval.getAttackingSquares()).count() * 100;

	// Holding center is good
	ret += (centerSquares & eval.getAttackingSquares()).count() * 100;
	ret += (centerSquares & eval.getOwnPieces()).count() * 100;

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
	for (int i = 0; i < node->childCount; ++i) {
		freeNode(node->children[i]);
	}

	if (node->bestChild != nullptr)
		freeNode(node->bestChild);

	delete node;
}

} // namespace vimlock
