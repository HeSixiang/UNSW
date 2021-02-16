#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/functional.hpp>
#include <vector>

#include "sort_descending.hpp"

TEST_CASE("sort_descending sorts in descending order") {
	SECTION("empty vector") {
		auto v = std::vector<std::string>();
		REQUIRE(v.empty());

		sort_descending(v);
		CHECK(ranges::is_sorted(v, ranges::greater{}));
	}

	SECTION("single-element vector") {
		auto v = std::vector<std::string>{"1"};
		REQUIRE(ranges::distance(v) == 1);

		sort_descending(v);
		CHECK(ranges::is_sorted(v, ranges::greater{}));
	}

	SECTION("many elements") {
		auto v = std::vector<std::string>{"36", "84", "122", "76", "41", "19"};
		REQUIRE(ranges::distance(v) > 1);

		sort_descending(v);
		CHECK(ranges::is_sorted(v, ranges::greater{}));
	}
}
