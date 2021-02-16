// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "cards.hpp"

#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <vector>

TEST_CASE("Last n elements of a range") {
	using comp6771::blue_number;
	using comp6771::blue_skip;
	using comp6771::card;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::yellow_draw_four;

	auto const hand = comp6771::deal_hand();
	REQUIRE(not hand.empty());

	namespace views = ranges::views;
	SECTION("Processing only the last _n_ elements") {
		auto final_two = hand | views::take_last(2);

		auto const expected = std::vector<card>{blue_number, blue_skip};
		CHECK(ranges::equal(final_two, expected));
	}

	SECTION("Processing all but the last _n_ elements") {
		auto first_six = hand | views::drop_last(2);

		auto const expected = std::vector<card>{
		   red_number,
		   blue_number,
		   green_draw_two,
		   blue_number,
		   blue_skip,
		   yellow_draw_four,
		   blue_number,
		};
		CHECK(ranges::equal(first_six, expected));
	}
}
