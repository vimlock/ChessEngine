#include "Engine.h"
#include "Eval.h"
#include "Move.h"
#include "Moves.h"

#include <cassert>
#include <cstddef>
#include <limits>
#include <algorithm>

namespace vimlock
{

// 4x4 region in center
static Bitboard centerSquares = Bitboard(0x3c3c000000LL);

// A and H file
static Bitboard edges = Bitboard(0x20c0c18181818181);

static Bitboard topAndBottomRanks = Bitboard::rank(RANK_8) | Bitboard::rank(RANK_1);

constexpr int PAWN_VALUE   = 1000;
constexpr int ROOK_VALUE   = 5000;
constexpr int KNIGHT_VALUE = 3000;
constexpr int BISHOP_VALUE = 3000;
constexpr int QUEEN_VALUE  = 9000;

/// Maximum number of moves a player can choose from during a single turn.
/// Theoretical maximum is 218 based on web search.
constexpr int maxMoves = 256;

Move Node::getMove() const
{
	return Move(src, dst, promote);
}

MoveList Node::getMoves() const
{
	MoveList ret;

	for (int i = 0; i < movesCount; ++i)
		ret.push_back(moves[i]);

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

	// traverse(root);
	traverse(root, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

	if (root->movesCount == 0) {
		freeNode(root);
		return false;
	}

	ret.best = root->moves[0];
	ret.eval = root->eval;
	ret.total = total;

	for (int i = 0; i < root->movesCount; ++i) {
		ret.continuation.push_back(root->moves[i]);
	}

	freeNode(root);

	return true;
}

void Engine::stop()
{
	// Nothing to do now as we're still single threaded
}

void Engine::traverse(Node *node, int alpha, int beta)
{
	if (node->depth >= maxDepth) {
		evaluate(node);
		return;
	}

	Bitboard allPieces = node->allPieces;
	Bitboard ownPieces = node->ownPieces;
	Bitboard oppPieces = node->oppPieces;

	// Generate possible moves from current position
	
	int possibleMovesCount = 0;
	MoveCandidate possibleMoves[256];
	
	for (uint64_t i = 0; i < 64; ++i) {

		assert(possibleMovesCount < maxMoves && "more moves than theoretically possible?");

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

			Square srcSquare = Square(i);
			Square dstSquare = Square(k);

			if (square.getPiece() == PAWN && (dstBitboard & (topAndBottomRanks))) {
				// Add possible promotions
				
				possibleMoves[possibleMovesCount++] = MoveCandidate({srcSquare, dstSquare, ROOK}, MOVE_PROMOTE);
				possibleMoves[possibleMovesCount++] = MoveCandidate({srcSquare, dstSquare, KNIGHT}, MOVE_PROMOTE);
				possibleMoves[possibleMovesCount++] = MoveCandidate({srcSquare, dstSquare, BISHOP}, MOVE_PROMOTE);
				possibleMoves[possibleMovesCount++] = MoveCandidate({srcSquare, dstSquare, QUEEN}, MOVE_PROMOTE);
			}
			else {
				// Regular move

				MoveOrder order = MOVE_REGULAR;
				if (dstBitboard & oppPieces)
					order = MOVE_CAPTURE;

				possibleMoves[possibleMovesCount++] = MoveCandidate({srcSquare, dstSquare}, order);

				if (square.getPiece() == KING) {
					// TODO: consider castling
				}
			}
		}
	}

	// TODO: sorting bucket based approach would be faster
	std::sort(possibleMoves, possibleMoves + possibleMovesCount, [](const MoveCandidate &a, const MoveCandidate &b) {
			return static_cast<int>(a.order) < static_cast<int>(b.order);
	});

	bool maximize = node->board.getCurrent() == board.getCurrent();
	node->eval = maximize ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

	Move bestMoves[32];
	int bestMovesCount = 0;

	int legalMoves = 0;

	for (int i = 0; i < possibleMovesCount; ++i) {

		Move move = possibleMoves[i].move;

		Node *child = allocNode();
		child->src = move.getSource();
		child->dst = move.getDestination();
		child->board = node->board;
		child->promote = move.getPromotion();
		child->depth = node->depth + 1;

		std::copy(node->moves, node->moves + node->movesCount, child->moves);
		child->movesCount = node->movesCount;
		child->moves[child->movesCount++] = child->getMove();

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
			continue;
		}

		legalMoves++;

		// Flipped around as the turn changed
		child->allPieces = allPieces;
		child->ownPieces = oppPieces;
		child->oppPieces = ownPieces;

		child->board.flipCurrent();

		traverse(child, alpha, beta);

		if (maximize) {
			if (child->eval > node->eval) {

				node->eval = child->eval;

				std::copy(child->moves, child->moves + child->movesCount, bestMoves);
				bestMovesCount = child->movesCount;
			}
			if (child->eval > alpha) {
				alpha = child->eval;
			}
		}
		else {
			if (child->eval < node->eval) {
				node->eval = child->eval;

				std::copy(child->moves, child->moves + child->movesCount, bestMoves);
				bestMovesCount = child->movesCount;
			}
			if (child->eval < beta) {
				beta = child->eval;
			}
		}

		freeNode(child);

		// Prune remaining branches
		if (alpha >= beta) {
			break;
		}
	}

	if (legalMoves == 0) {
		Bitboard attackedSquares = getAvailableCaptures(node->board, node->allPieces, node->oppPieces);
		Bitboard ownKing = node->board.getPieces(node->board.getCurrent(), KING);

		// Stalemate?
		if (!(ownKing & attackedSquares)) {
			node->eval = 0;
			return;
		}
		else if (board.getCurrent() == node->board.getCurrent()) {
			// Opponent checkmated us, try to struggle until the end
			node->eval = std::numeric_limits<int>::min() + node->depth;
		}
		else {
			// Opponent got checkmated, prefer shorter checkmates
			node->eval = std::numeric_limits<int>::max() - node->depth;
		}
	}
	else {
		std::copy(bestMoves, bestMoves + bestMovesCount, node->moves);
		node->movesCount = bestMovesCount;
	}
}

void Engine::evaluate(Node *node)
{
	int own = getScore(node->board, board.getCurrent());
	int opp = getScore(node->board, BoardUtils::getOpponent(board.getCurrent()));

	node->eval = own - opp;

	total++;
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

	// Doubled pawns are bad
	for (int i = FILE_A; i <= FILE_H; ++i) {
		if ((eval.getOwnPieces() & Bitboard::file(i)).count() > 1) {
			ret -= 100;
		}
	}

	// Shielded king is good
	Square king = eval.getOwnKing().findFirstSquare();
	int guards = (eval.getOwnPieces() & Bitboard::adjacent(king)).count();
	if (guards > 2)
		ret += 200;
	else if (guards == 1)
		ret += 100;

	return ret;
}

int Engine::getPieceValue(Piece piece) const
{
	switch (piece) {
		case PAWN:   return PAWN_VALUE;
		case ROOK:   return ROOK_VALUE;
		case KNIGHT: return KNIGHT_VALUE;
		case BISHOP: return BISHOP_VALUE;
		case QUEEN:  return QUEEN_VALUE;
		case KING:   return 0; // Worth nothing, yet everything.
	}

	assert(false && "Should be unreachable");
	return 0;
}

Node * Engine::allocNode()
{
	Node *node = new Node();

	node->depth = 0;
	node->movesCount = 0;
	node->promote = PAWN;
	node->eval = 0;

	return node;
}

void Engine::freeNode(Node *node)
{
	delete node;
}

} // namespace vimlock
