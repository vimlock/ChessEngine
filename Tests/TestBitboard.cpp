#include <catch2/catch.hpp>

#include "Bitboard.h"

using namespace vimlock;

TEST_CASE("Bitboard constructors", "[Bitboard]")
{
	SECTION("Default constructor initializes to 0") {
		REQUIRE(Bitboard().count() == 0);
		REQUIRE(Bitboard().empty());
	}

	// In case the compiler decides to choose Bitboard(uint64_t) constructor
	SECTION("No implicit conversion weirdness") {
		REQUIRE(Bitboard(H1).count() == 1);
		REQUIRE(Bitboard(H8).count() == 1);
		REQUIRE(Bitboard(A8).count() == 1);
	}

	SECTION("Single bit constructor works") {
		REQUIRE(Bitboard(Square(5, 5)).count() == 1);
		REQUIRE(Bitboard(Square(5, 5)).contains(5, 5));
		REQUIRE(Bitboard(Square(7, 0)).count() == 1);
	}

	SECTION("Full constructor sets all bits") {
		REQUIRE(Bitboard(0xFF00FF00FF00FF00ULL).count() == 32);
	}

	SECTION("Rank constructor") {
		REQUIRE(Bitboard::rank(RANK_1).count() == 8);
		REQUIRE(Bitboard::rank(RANK_1).contains(FILE_A, RANK_1));
		REQUIRE(!Bitboard::rank(RANK_1).contains(FILE_A, RANK_2));

		REQUIRE(Bitboard::rank(RANK_8).count() == 8);
		REQUIRE(Bitboard::rank(RANK_8).contains(FILE_A, RANK_8));
		REQUIRE(Bitboard::rank(RANK_8).contains(FILE_H, RANK_8));
		REQUIRE(!Bitboard::rank(RANK_8).contains(FILE_A, RANK_7));
	}

	SECTION("File constructor") {
		REQUIRE(Bitboard::file(FILE_A).count() == 8);
		REQUIRE(Bitboard::file(FILE_A).contains(FILE_A, RANK_1));
		REQUIRE(Bitboard::file(FILE_A).contains(FILE_A, RANK_8));
		REQUIRE(!Bitboard::file(FILE_A).contains(FILE_B, RANK_1));
		REQUIRE(!Bitboard::file(FILE_A).contains(FILE_B, RANK_8));
	}
}

TEST_CASE("Bitboard ops")
{
	SECTION("Invert works") {
		REQUIRE(Bitboard().inverted().count() == 64);
		REQUIRE(Bitboard(0xFFFFFFFF00000000ULL).count() == 32);
	}

	SECTION("contains() works") {
		REQUIRE(Bitboard(Square(7, 7)).contains(7,7));
		REQUIRE(Bitboard(Square(0, 5)).contains(0,5));
		REQUIRE(!Bitboard(Square(5, 0)).contains(0,0));
	}

	SECTION("overlaps() works") {
		REQUIRE(Bitboard(0xFF00ULL).overlaps(Bitboard(0x100ULL)));
		REQUIRE(!Bitboard(0xFF00ULL).overlaps(Bitboard(0xFF0000ULL)));
	}

	SECTION("count() works") {
		REQUIRE(Bitboard(0x0ULL).count() == 0);
		REQUIRE(Bitboard(0xFFULL).count() == 8);
	}

	SECTION("operator | works") {
		REQUIRE((Bitboard(Square(2, 3)) | Bitboard(Square(3, 2))).count() == 2);
	}

	SECTION("operator & work") {
		REQUIRE((Bitboard(0xFF00FFULL) & Bitboard(0xFFFF00ULL)).count() == 8);
	}

	SECTION("bool cast works") {
		REQUIRE(!Bitboard());
		REQUIRE(Bitboard(0x1ULL));
		REQUIRE(Bitboard(0xFFFFFFFFFFULL));
	}
}

TEST_CASE("Bitboard flip ranks")
{
	REQUIRE(Bitboard(Square(0, 0)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(0, 0)).flipRanks().contains(0, 7));

	REQUIRE(Bitboard(Square(0, 7)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(0, 7)).flipRanks().contains(0, 0));

	REQUIRE(Bitboard(Square(7, 0)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(7, 0)).flipRanks().contains(7, 7));

	REQUIRE(Bitboard(Square(0, 1)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(0, 1)).flipRanks().contains(0, 6));

	REQUIRE(Bitboard(Square(0, 2)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(0, 2)).flipRanks().contains(0, 5));

	REQUIRE(Bitboard(Square(0, 3)).flipRanks().count() == 1);
	REQUIRE(Bitboard(Square(0, 3)).flipRanks().contains(0, 4));
}
