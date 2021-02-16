// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <vector>

#include "cards.hpp"

TEST_CASE("Non-mutating transformation") {
	using comp6771::card;
	using comp6771::colour;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::value;
	using comp6771::yellow_draw_four;

	auto const hand = comp6771::deal_hand();
	REQUIRE(not hand.empty());

	namespace views = ranges::views;
	SECTION("What if we already have a vector?") {
		auto swap_blue = [](card const c) {
			return c.colour != colour::blue ? c : card{colour::green, c.value};
		};

		auto const green_number = card{colour::green, value::number};
		auto const green_skip = card{colour::green, value::skip};
		auto const expected = std::vector<card>{
		   red_number,
		   green_number,
		   green_draw_two,
		   green_number,
		   green_skip,
		   yellow_draw_four,
		   green_number,
		   green_number,
		   green_skip,
		};
		CHECK(ranges::equal(hand | views::transform(swap_blue), expected));
	}
}
