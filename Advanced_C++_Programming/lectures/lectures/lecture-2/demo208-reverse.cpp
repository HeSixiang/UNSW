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

TEST_CASE("Reversing a sequence on demand") {
	using comp6771::blue_number;
	using comp6771::blue_skip;
	using comp6771::card;
	using comp6771::colour;

	auto const hand = comp6771::deal_hand();
	REQUIRE(not hand.empty());
	REQUIRE(hand[1] == blue_number);
	REQUIRE(hand[8] == blue_skip);

	namespace views = ranges::views;
	auto const a_blue_card = [](card const& c) { return c.colour == colour::blue; };
	{
		auto const result = ranges::find_if(hand, a_blue_card);
		REQUIRE(result != hand.end());
		CHECK(*result == blue_number);
	}
	{
		// description: "I want to process `hand` in reverse order."
		auto back_to_front = hand | views::reverse;

		// action: "I want to find the last blue card.""
		auto const result = ranges::find_if(back_to_front, a_blue_card);
		REQUIRE(result != back_to_front.end());
		CHECK(*result == blue_skip);
	}
	{ // hand left unchanged by the previous block
		auto const result = ranges::find_if(hand, a_blue_card);
		REQUIRE(result != hand.end());
		CHECK(*result == blue_number);
	}
}
