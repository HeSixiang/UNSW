// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <vector>

#include "cards.hpp"

TEST_CASE("First n elements of a range") {
	using comp6771::blue_number;
	using comp6771::blue_skip;
	using comp6771::card;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::yellow_draw_four;

	auto const hand = comp6771::deal_hand();
	REQUIRE(not hand.empty());
	REQUIRE(hand[1] == blue_number);
	REQUIRE(hand[8] == blue_skip);

	namespace views = ranges::views;
	SECTION("Processing only the first _n_ elements") {
		// Description: I want to work with at most the first three elements in `hand`.
		auto front_3 = hand | views::take(3);

		auto const expected = std::vector<card>{red_number, blue_number, green_draw_two};
		CHECK(ranges::equal(front_3, expected));
	}

	SECTION("Processing all but the first _n_ elements") {
		// Description: I want to work with all but the first three elements in `hand`.
		auto back_6 = hand | views::drop(3);

		auto const expected = std::vector<card>{
		   blue_number,
		   blue_skip,
		   yellow_draw_four,
		   blue_number,
		   blue_number,
		   blue_skip,
		};

		CHECK(ranges::equal(back_6, expected));
	}
}
