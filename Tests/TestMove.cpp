#include <catch2/catch.hpp>
#include "Engine/Move.h"

using namespace vimlock;
using namespace RankAndFile;

TEST_CASE("Move to LAN string")
{
	REQUIRE(Move(A1, A2).toLan() == "a1a2");
	REQUIRE(Move(A8, A1).toLan() == "a8a1");
	REQUIRE(Move(H1, A8).toLan() == "h1a8");
	REQUIRE(Move(H8, A1).toLan() == "h8a1");
}

TEST_CASE("Move equality operators")
{
	REQUIRE(Move(A1, A2) == Move(A1, A2));
	REQUIRE(Move(H1, D3) == Move(H1, D3));
	REQUIRE(Move(H7, D3) != Move(H1, D3));
	REQUIRE(Move(H1, D4) != Move(H1, D3));
}

TEST_CASE("Move parse valid LAN")
{
	Move move;

	SECTION("Basic") {
		REQUIRE(move.parseLan("a1a2"));
		REQUIRE(move == Move(A1, A2));

		REQUIRE(move.parseLan("h1h2"));
		REQUIRE(move == Move(H1, H2));

		REQUIRE(move.parseLan("h8h2"));
		REQUIRE(move == Move(H8, H2));
	}

	SECTION("Promote") {
		REQUIRE(move.parseLan("a2a1q"));
		REQUIRE(move == Move(A2, A1, QUEEN));

		REQUIRE(move.parseLan("h2h1n"));
		REQUIRE(move == Move(H2, H1, KNIGHT));

		REQUIRE(move.parseLan("h7h8b"));
		REQUIRE(move == Move(H7, H8, BISHOP));

		REQUIRE(move.parseLan("h7h8r"));
		REQUIRE(move == Move(H7, H8, ROOK));
	}
}

TEST_CASE("Move parse reject invalid LAN")
{
	SECTION("Plain wrong") {
		REQUIRE(!Move().parseLan("kissa"));
		REQUIRE(!Move().parseLan("fooo"));
		REQUIRE(!Move().parseLan("a1xx"));
		REQUIRE(!Move().parseLan("xxa1"));
		REQUIRE(!Move().parseLan(""));
		REQUIRE(!Move().parseLan("1a1a"));
		REQUIRE(!Move().parseLan("????"));
	}

	SECTION("Bad promotions") {
		REQUIRE(!Move().parseLan("a2a1!"));
		REQUIRE(!Move().parseLan("a2a1k"));
		REQUIRE(!Move().parseLan("a2a1v"));
		REQUIRE(!Move().parseLan("a2a1p"));
		REQUIRE(!Move().parseLan("a2a1qq"));
		REQUIRE(!Move().parseLan("a2a1rr"));
	}
}
