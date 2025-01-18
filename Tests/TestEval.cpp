#include <catch2/catch.hpp>
#include "Engine/Eval.h"

using namespace vimlock;
using namespace RankAndFile;

TEST_CASE("Pawn moves")
{
	SECTION("White double move for on first rank") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(A2, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A2).count() == 2);

		board.clear();
		board.setSquare(B2, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(B2).count() == 2);
	}

	SECTION("White double move blocked but single available") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(A2, WHITE, PAWN);
		board.setSquare(A4, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A2).count() == 1);
	}

	SECTION("White blocked") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(A2, WHITE, PAWN);
		board.setSquare(A3, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A2).count() == 0);
	}

	SECTION("White blocked but can capture") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(A2, WHITE, PAWN);
		board.setSquare(A3, BLACK, PAWN);
		board.setSquare(B3, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A2).count() == 1);
	}

	SECTION("Black double move for on first rank") {
		Board board;
		board.setCurrent(BLACK);

		board.clear();
		board.setSquare(A7, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 2);

		board.clear();
		board.setSquare(B7, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(B7).count() == 2);
	}

	SECTION("Black double move blocked but single available") {
		Board board;
		board.setCurrent(BLACK);

		board.clear();
		board.setSquare(A7, BLACK, PAWN);
		board.setSquare(A5, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 1);
	}

	SECTION("Black blocked") {
		Board board;
		board.setCurrent(BLACK);

		board.clear();
		board.setSquare(A7, BLACK, PAWN);
		board.setSquare(A6, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 0);
	}

	SECTION("Black blocked but can capture") {
		Board board;
		board.setCurrent(BLACK);

		board.clear();
		board.setSquare(A7, BLACK, PAWN);
		board.setSquare(A6, WHITE, PAWN);
		board.setSquare(B6, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 1);
	}

	SECTION("White pawn can promote") {
		Board board;
		board.setCurrent(WHITE);

		board.setSquare(A7, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 1);
	}

	SECTION("White pawn can capture and promote") {
		Board board;
		board.setCurrent(WHITE);

		board.setSquare(A7, WHITE, PAWN);
		board.setSquare(A8, BLACK, PAWN);
		board.setSquare(B8, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A7).count() == 1);
	}

	SECTION("Black pawn can promote and capture") {
		Board board;
		board.setCurrent(BLACK);

		board.setSquare(A2, BLACK, PAWN);
		board.setSquare(A1, WHITE, PAWN);
		board.setSquare(B1, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A2).count() == 1);
	}
}

TEST_CASE("Rook moves")
{
	SECTION("Unblocked moves") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 14);

		board.clear();
		board.setSquare(H1, WHITE, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 14);

		board.clear();
		board.setSquare(A8, WHITE, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 14);

		board.clear();
		board.setSquare(H8, WHITE, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 14);

		board.clear();
		board.setSquare(C2, WHITE, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(C2).count() == 14);
	}

	SECTION("blocked moves") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, ROOK);
		board.setSquare(B1, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 7);

		board.clear();
		board.setSquare(H1, WHITE, ROOK);
		board.setSquare(G1, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 7);

		board.clear();
		board.setSquare(A8, WHITE, ROOK);
		board.setSquare(A7, WHITE, PAWN);
		board.setSquare(B8, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 0);

		board.clear();
		board.setSquare(H8, WHITE, ROOK);
		board.setSquare(H7, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 7);

		board.clear();
		board.setSquare(C2, WHITE, ROOK);
		board.setSquare(C1, WHITE, PAWN);
		board.setSquare(C3, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(C2).count() == 7);
	}
}

TEST_CASE("Knight moves")
{
	SECTION("On corners") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 2);

		board.clear();
		board.setSquare(H1, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 2);

		board.clear();
		board.setSquare(H8, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 2);

		board.clear();
		board.setSquare(A8, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 2);
	}

	SECTION("One square from corners") {
		Board board;

		board.clear();
		board.setSquare(B2, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(B2).count() == 4);

		board.clear();
		board.setSquare(G2, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(G2).count() == 4);

		board.clear();
		board.setSquare(G7, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(G7).count() == 4);

		board.clear();
		board.setSquare(B7, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(B7).count() == 4);
	}

	SECTION("In middle") {
		Board board;

		board.clear();
		board.setSquare(D4, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(D4).count() == 8);

		board.clear();
		board.setSquare(E5, WHITE, KNIGHT);
		REQUIRE(MoveEval(board).getAvailableMoves(E5).count() == 8);
	}

	SECTION("Blocked by own") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, KNIGHT);
		board.setSquare(B3, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 1);

		board.clear();
		board.setSquare(A1, WHITE, KNIGHT);
		board.setSquare(B3, WHITE, PAWN);
		board.setSquare(C2, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 0);
	}

	SECTION("Not blocked by opponent pieces") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, KNIGHT);
		board.setSquare(B3, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 2);

		board.clear();
		board.setSquare(A1, WHITE, KNIGHT);
		board.setSquare(B3, BLACK, PAWN);
		board.setSquare(C2, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 2);
	}
}

TEST_CASE("Bishop moves")
{
	SECTION("On corners unblocked") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 7);

		board.clear();
		board.setSquare(H1, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 7);
		
		board.clear();
		board.setSquare(A8, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 7);

		board.clear();
		board.setSquare(H8, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 7);
	}

	SECTION("One from edge unblocked") {
		Board board;

		board.clear();
		board.setSquare(B2, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(B2).count() == 9);

		board.clear();
		board.setSquare(G2, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(G2).count() == 9);

		board.clear();
		board.setSquare(G7, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(G7).count() == 9);

		board.clear();
		board.setSquare(B7, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(B7).count() == 9);
	}

	SECTION("In middle unblocked") {
		Board board;

		board.clear();
		board.setSquare(D4, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(D4).count() == 13);

		board.clear();
		board.setSquare(D5, WHITE, BISHOP);
		REQUIRE(MoveEval(board).getAvailableMoves(D5).count() == 13);
	}

	SECTION("On corners blocked") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, BISHOP);
		board.setSquare(B2, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 0);

		board.clear();
		board.setSquare(H1, WHITE, BISHOP);
		board.setSquare(G2, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 0);
		
		board.clear();
		board.setSquare(A8, WHITE, BISHOP);
		board.setSquare(B7, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 0);

		board.clear();
		board.setSquare(H8, WHITE, BISHOP);
		board.setSquare(G7, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 0);
	}

	SECTION("On corners blocked but can capture") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, BISHOP);
		board.setSquare(B2, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 1);

		board.clear();
		board.setSquare(H1, WHITE, BISHOP);
		board.setSquare(G2, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 1);
		
		board.clear();
		board.setSquare(A8, WHITE, BISHOP);
		board.setSquare(B7, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A8).count() == 1);

		board.clear();
		board.setSquare(H8, WHITE, BISHOP);
		board.setSquare(G7, BLACK, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 1);
	}
}

// Internally implemented as rook+bishop but sanity checking is always good
TEST_CASE("Queen moves")
{
	SECTION("On corners unblocked") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, QUEEN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 21);

		board.clear();
		board.setSquare(H1, WHITE, QUEEN);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 21);

		board.clear();
		board.setSquare(H8, WHITE, QUEEN);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 21);

		board.clear();
		board.setSquare(A1, WHITE, QUEEN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 21);
	}
}

TEST_CASE("King moves")
{
	SECTION("On corners unblocked") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, KING);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 3);

		board.clear();
		board.setSquare(H1, WHITE, KING);
		REQUIRE(MoveEval(board).getAvailableMoves(H1).count() == 3);

		board.clear();
		board.setSquare(H8, WHITE, KING);
		REQUIRE(MoveEval(board).getAvailableMoves(H8).count() == 3);

		board.clear();
		board.setSquare(A1, WHITE, KING);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 3);
	}

	SECTION("On corners blocked") {
		Board board;

		board.clear();
		board.setSquare(A1, WHITE, KING);
		board.setSquare(B2, WHITE, PAWN);
		board.setSquare(A2, WHITE, PAWN);
		board.setSquare(B1, WHITE, PAWN);
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 0);
	}

	SECTION("King can't move to check") {
		Board board;

		board.clear();
		board.setSquare(D1, WHITE, KING);
		board.setSquare(C8, BLACK, ROOK);
		board.setSquare(E8, BLACK, ROOK);
		REQUIRE(MoveEval(board).getAvailableMoves(D1).count() == 1);
	}
}

TEST_CASE("King in check")
{
	SECTION("By pawn") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(E4, WHITE, KING);
		board.setSquare(D5, BLACK, PAWN);
		REQUIRE(MoveEval(board).isInCheck());

		board.clear();
		board.setSquare(E4, WHITE, KING);
		board.setSquare(F5, BLACK, PAWN);
		REQUIRE(MoveEval(board).isInCheck());
	}

	SECTION("By rook") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(D8, BLACK, ROOK);
		board.setSquare(D1, WHITE, KING);
		REQUIRE(MoveEval(board).isInCheck());

		board.clear();
		board.setSquare(A1, BLACK, ROOK);
		board.setSquare(D1, WHITE, KING);
		REQUIRE(MoveEval(board).isInCheck());

		board.clear();
		board.setSquare(E8, BLACK, ROOK);
		board.setSquare(D1, WHITE, KING);
		REQUIRE(!MoveEval(board).isInCheck());

		board.clear();
		board.setSquare(A2, BLACK, ROOK);
		board.setSquare(D1, WHITE, KING);
		REQUIRE(!MoveEval(board).isInCheck());
	}

	SECTION("By knight") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(E4, WHITE, KING);
		board.setSquare(G3, BLACK, KNIGHT);
		REQUIRE(MoveEval(board).isInCheck());

		board.clear();
		board.setSquare(E4, WHITE, KING);
		board.setSquare(F6, BLACK, KNIGHT);
		REQUIRE(MoveEval(board).isInCheck());
	}

	SECTION("King can capture out of check") {
		Board board;
		board.setCurrent(WHITE);

		board.clear();
		board.setSquare(A1, WHITE, KING);
		board.setSquare(B2, BLACK, QUEEN);

		REQUIRE(MoveEval(board).isInCheck());
		REQUIRE(MoveEval(board).getAvailableMoves(A1).count() == 1);
	}
}
