// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <compare>
#include <vector>

#include "cards.hpp"

TEST_CASE("Removing select elements on demand") {
	using comp6771::blue_number;
	using comp6771::blue_skip;
	using comp6771::card;
	using comp6771::colour;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::yellow_draw_four;

	auto const hand = comp6771::deal_hand();
	REQUIRE(not hand.empty());

	namespace views = ranges::views;
	auto is_blue = [](card const c) { return c.colour == colour::blue; };

	SECTION("views::filter --- Consider only the blue cards") {
		// description: I want to process only my blue cards.
		auto all_blue = hand | views::filter(is_blue);
		CHECK(ranges::distance(all_blue) == 6);
		CHECK(all_blue.front() == blue_number);
		CHECK(all_blue.back() == blue_skip);

		auto const expected = std::vector<card>{
		   blue_number,
		   blue_number,
		   blue_skip,
		   blue_number,
		   blue_number,
		   blue_skip,
		};

		auto const actual = all_blue | ranges::to<std::vector>;
		CHECK(expected == actual);
	}

	SECTION("views::remove_if --- Don't consider the blue cards") {
		// description: I want to process only my non-blue cards.
		auto no_blue = hand | views::remove_if(is_blue);
		CHECK(ranges::distance(no_blue) == 3);
		CHECK(no_blue.front() == red_number);
		CHECK(no_blue.back() == yellow_draw_four);

		auto const expected = std::vector<card>{
		   red_number,
		   green_draw_two,
		   yellow_draw_four,
		};

		auto const actual = no_blue | ranges::to<std::vector>;
		CHECK(expected == actual);
	}
}
