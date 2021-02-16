// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <string>
#include <vector>

#include <queue>

TEST_CASE("Copying values from one range to another, already existing range") {
	auto q = std::queue<std::string>();

	auto const from = std::string("Hello there");
	auto const expected = std::vector<char>{'H', 'e', 'l', 'l', 'o', ' ', 't', 'h', 'e', 'r', 'e'};

	SECTION("Destination has the same room as its source") {
		auto to = std::vector<char>(from.size());
		REQUIRE(ranges::distance(from) == ranges::distance(to));

		ranges::copy(from, to.begin());
		CHECK(to == expected);
	}

	SECTION("Destination has more room than its source") {
		auto to = std::vector<char>(from.size() + 1);
		REQUIRE(ranges::distance(from) < ranges::distance(to));

		ranges::copy(from, to.begin());
		CHECK(ranges::equal(expected.begin(), expected.end(), to.begin(), ranges::prev(to.end())));
	}

	// Uncomment me to find out what happens!
	// Try compiling in Debug and Release mode to see if the same thing happens.
	// SECTION("Things go wrong when we don't have enough room in our destination") {
	// 	auto to = std::vector<char>(5);
	// 	REQUIRE(ranges::distance(from) > ranges::distance(to));

	// 	ranges::copy(from, to.begin());
	// 	CHECK(to == expected);
	// }

	SECTION("Destination has less room than the source, but we account for it") {
		auto to = std::vector<char>();
		REQUIRE(to.empty());

		ranges::copy(from, ranges::back_inserter(to));
		CHECK(to == expected);
	}

	SECTION("Destination has less room than the source, and isn't empty") {
		SECTION("Easy way, works for both vector and string") {
			auto to = std::vector<char>(5);
			REQUIRE(ranges::distance(from) > ranges::distance(to));
			REQUIRE(not to.empty());

			to.assign(from.begin(), from.end());
			CHECK(to == expected);
		}

		SECTION("General way, not necessary to know for COMP6771; but might be a curiosity") {
			// Will be revealed in Week 8 if someone reminds me!
		}
	}
}
