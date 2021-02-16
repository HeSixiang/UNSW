// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/functional.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/range.hpp>
#include <compare>
#include <vector>

#include "cards.hpp"

TEST_CASE("Simple algorithms") {
	using comp6771::blue_number;
	using comp6771::blue_skip;
	using comp6771::card;
	using comp6771::colour;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::value;
	using comp6771::yellow_draw_four;

	auto hand = comp6771::deal_hand();

	REQUIRE(not hand.empty());

	SECTION("Counting cards") {
		// note: `count` requires `card1 == card2` to compile
		CHECK(ranges::count(hand, red_number) == 1);
		CHECK(ranges::count(hand, blue_number) == 4);
		CHECK(ranges::count(hand, blue_skip) == 2);
	}

	SECTION("Finding a card in an unsorted sequence") {
		// note: `find` requires `card1 == card2` to compile
		auto playing_card = ranges::find(hand, blue_number);
		REQUIRE(playing_card != hand.end());
		CHECK(*playing_card == blue_number);

		playing_card = hand.erase(playing_card); // we remove a card from our hand when we play it
		REQUIRE(playing_card != hand.end());
		CHECK(*playing_card == green_draw_two);

		playing_card = ranges::find(playing_card, hand.end(), yellow_draw_four);
		REQUIRE(playing_card != hand.end());
		CHECK(*playing_card == yellow_draw_four);
		playing_card = hand.erase(playing_card);

		playing_card = ranges::find(playing_card, hand.end(), yellow_draw_four);
		REQUIRE(playing_card == hand.end());

		CHECK(ranges::find(hand, yellow_draw_four) == hand.end());
	}

	SECTION("Finding two adjacent cards that are the same") {
		auto const first_equivalent = ranges::adjacent_find(hand);
		REQUIRE(first_equivalent != hand.end());
		CHECK(*first_equivalent == blue_number);

		auto const second_equivalent = ranges::next(first_equivalent);
		REQUIRE(second_equivalent != hand.end());
		CHECK(*second_equivalent == blue_number);
	}
}
