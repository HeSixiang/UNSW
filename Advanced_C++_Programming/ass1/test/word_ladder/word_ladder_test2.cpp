#include "comp6771/word_ladder.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "comp6771/testing/range/contain.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/is_sorted.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/range/primitives.hpp"

TEST_CASE("abduction -> abducting (no path)") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const from = std::string{"abduction"};
	auto const to = std::string{"abducting"};

	// check the assumptions
	REQUIRE(english_lexicon.contains(from));
	REQUIRE(english_lexicon.contains(to));
	REQUIRE(from != to);
	REQUIRE(ranges::size(from) == ranges::size(to));

	auto const ladders = word_ladder::generate(from, to, english_lexicon);

	SECTION("there is no path from abduction to abducting") {
		CHECK(ranges::size(ladders) == 0);
	}
}

TEST_CASE("airplane -> tricycle (no path)") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const from = std::string{"airplane"};
	auto const to = std::string{"tricycle"};

	// check the assumptions
	REQUIRE(english_lexicon.contains(from));
	REQUIRE(english_lexicon.contains(to));
	REQUIRE(from != to);
	REQUIRE(ranges::size(from) == ranges::size(to));

	auto const ladders = word_ladder::generate(from, to, english_lexicon);

	SECTION("there is no path from airplane to tricycle") {
		CHECK(ranges::size(ladders) == 0);
	}
}
