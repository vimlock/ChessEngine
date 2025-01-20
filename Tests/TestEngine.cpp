#include <catch2/catch.hpp>
#include "Engine.h"
#include "Format.h"

using namespace vimlock;

MoveList bestMoves(const Board &board, size_t maxCount, int depth=2)
{
	Engine engine{depth};
	engine.setPosition(board);

	Evaluation eval;
	engine.poll(eval);

	if (eval.continuation.size() > maxCount)
		eval.continuation.resize(maxCount);

	return eval.continuation;
}

TEST_CASE("Stalemate")
{
	Board board;

	SECTION("King+King") {
		board.setSquare(B1, WHITE, KING);
		board.setSquare(B2, BLACK, PAWN);
		board.setSquare(B3, BLACK, KING);
		REQUIRE(bestMoves(board, 3, 3) == MoveList{});
	}
}

TEST_CASE("Find a mate in one")
{
	Board board;

	SECTION("King+Queen vs. King") {
		board.setSquare(A1, BLACK, KING);
		board.setSquare(C3, WHITE, KING);
		board.setSquare(H2, WHITE, QUEEN);

		REQUIRE(bestMoves(board, 3, 2) == MoveList{{H2, B2}});
	}
}

TEST_CASE("Find a mate in two")
{
	Board board;

	SECTION("King+Queen vs. King") {
		board.setSquare(B1, WHITE, KING);
		board.setSquare(E1, WHITE, ROOK);
		board.setSquare(E2, WHITE, ROOK);

		board.setSquare(H8, BLACK, KING);
		board.setSquare(E8, BLACK, ROOK);
		board.setSquare(A8, BLACK, ROOK);
		board.setSquare(H7, BLACK, PAWN);
		board.setSquare(G7, BLACK, PAWN);
		board.setSquare(F7, BLACK, PAWN);

		INFO(""<<toString(board));

		REQUIRE(bestMoves(board, 4, 4) == MoveList{{E2,E8}, {A8,E8}, {E1, E8 }});
	}
}

TEST_CASE("Promote optimally")
{
	Board board;

	SECTION("Queen is best") {
		board.setSquare(H7, WHITE, PAWN);
		board.setSquare(A7, BLACK, KING);
		board.setSquare(A1, WHITE, KING);

		REQUIRE(bestMoves(board, 1) == MoveList{ {H7, H8, QUEEN}});
	}
}
