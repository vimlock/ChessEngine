#include <catch2/catch.hpp>

#include "Engine/Board.h"
#include "Engine/Format.h"

using namespace vimlock;

TEST_CASE("Castling")
{
	Board board;

	SECTION("Castle rights exist on start") {
		board.setStandardPosition();
		REQUIRE(board.canCastle(G1));
		REQUIRE(board.canCastle(C1));
		REQUIRE(board.canCastle(G8));
		REQUIRE(board.canCastle(C8));
	}

	SECTION("White short castling works") {
		board.setStandardPosition();

		// Remove pieces out of the way
		board.setSquare(F1, SquareState());
		board.setSquare(G1, SquareState());

		REQUIRE(board.canCastle(G1));
		REQUIRE(board.movePiece(E1, G1));
		REQUIRE(board.getSquare(G1).getPiece() == KING);
		REQUIRE(board.getSquare(F1).getPiece() == ROOK);

		SECTION("White should lose castle rights") {
			REQUIRE(!board.canCastle(G1));
			REQUIRE(!board.canCastle(C1));
		}

		SECTION("Black castle rights should be unaffected") {
			REQUIRE(board.canCastle(G8));
			REQUIRE(board.canCastle(C8));
		}
	}

	SECTION("White long castling works") {
		board.setStandardPosition();
		
		// Remove pieces out of the way
		board.setSquare(B1, SquareState());
		board.setSquare(C1, SquareState());
		board.setSquare(D1, SquareState());

		REQUIRE(board.canCastle(C1));
		REQUIRE(board.movePiece(E1, C1));
		REQUIRE(board.getSquare(D1).getPiece() == ROOK);
		REQUIRE(board.getSquare(C1).getPiece() == KING);

		SECTION("White should lose castle rights") {
			REQUIRE(!board.canCastle(G1));
			REQUIRE(!board.canCastle(C1));
		}

		SECTION("Black castle rights should be unaffected") {
			REQUIRE(board.canCastle(G8));
			REQUIRE(board.canCastle(C8));
		}
	}

	SECTION("Black short castling works") {
		board.setStandardPosition();

		// Remove pieces out of the way
		board.setSquare(F8, SquareState());
		board.setSquare(G8, SquareState());

		REQUIRE(board.canCastle(G8));
		REQUIRE(board.movePiece(E8, G8));
		REQUIRE(board.getSquare(G8).getPiece() == KING);
		REQUIRE(board.getSquare(F8).getPiece() == ROOK);

		SECTION("Black should lose castle rights") {
			REQUIRE(!board.canCastle(G8));
			REQUIRE(!board.canCastle(C8));
		}

		SECTION("White castle rights should be unaffected") {
			REQUIRE(board.canCastle(G1));
			REQUIRE(board.canCastle(C1));
		}
	}

	SECTION("Black long castling works") {
		board.setStandardPosition();

		// Remove pieces out of the way
		board.setSquare(B8, SquareState());
		board.setSquare(C8, SquareState());
		board.setSquare(D8, SquareState());

		REQUIRE(board.canCastle(C8));
		REQUIRE(board.movePiece(E8, C8));
		REQUIRE(board.getSquare(C8).getPiece() == KING);
		REQUIRE(board.getSquare(D8).getPiece() == ROOK);

		SECTION("Black should lose castle rights") {
			REQUIRE(!board.canCastle(G8));
			REQUIRE(!board.canCastle(C8));
		}

		SECTION("White castle rights should be unaffected") {
			REQUIRE(board.canCastle(G1));
			REQUIRE(board.canCastle(C1));
		}
	}

	SECTION("Moving loses castle rights") {
		Board board;

		board.setSquare(H1, WHITE, ROOK);
		board.setSquare(A1, WHITE, ROOK);
		board.setSquare(E1, WHITE, KING);

		REQUIRE(board.canCastle(C1));
		REQUIRE(board.canCastle(G1));

		SECTION("Moving queen side rook") {
			REQUIRE(board.movePiece(A1, A2));
			REQUIRE(!board.canCastle(C1));
			REQUIRE(board.canCastle(G1));

			REQUIRE(board.movePiece(A2, A1));
			REQUIRE(!board.canCastle(C1));
			REQUIRE(board.canCastle(G1));
		}

		SECTION("Moving king side rook") {
			REQUIRE(board.movePiece(H1, H2));
			REQUIRE(!board.canCastle(G1));
			REQUIRE(board.canCastle(C1));

			REQUIRE(board.movePiece(H2, H1));
			REQUIRE(!board.canCastle(G1));
			REQUIRE(board.canCastle(C1));
		}

		SECTION("Moving king") {
			REQUIRE(board.movePiece(E1, E2));
			REQUIRE(!board.canCastle(G1));
			REQUIRE(!board.canCastle(C1));

			REQUIRE(board.movePiece(E2, E1));
			REQUIRE(!board.canCastle(G1));
			REQUIRE(!board.canCastle(C1));
		}
	}
}
