// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <map>

#include "cards.hpp"

auto note_swaps(std::map<comp6771::card, int>& cards_swapped, comp6771::card const c) -> void {
	auto result = cards_swapped.find(c);
	if (result == cards_swapped.end()) {
		cards_swapped.emplace(c, 1);
		return;
	}

	++(result->second);
}

TEST_CASE("Lambda expressions") {
	using comp6771::card;
	using comp6771::colour;

	auto hand = comp6771::deal_hand();

	REQUIRE(not hand.empty());

	SECTION("Simple lambda (how many blue cards are there?)") {
		auto const blue_cards = ranges::count_if(hand, [](card const c) { //
			return c.colour == colour::blue;
		});

		auto const expected_blue_cards = 6;
		CHECK(blue_cards == expected_blue_cards);
	}

	SECTION("Lambda with capture-by-value (surprise binary search)") {
		ranges::sort(hand);
		REQUIRE(ranges::is_sorted(hand));

		auto const blue_number = comp6771::blue_number; // Here because we can't capture globals
		auto [first, last] = ranges::equal_range(hand, blue_number);
		REQUIRE(first != last);
		CHECK(ranges::distance(first, last) == 4);

		CHECK(ranges::all_of(first, last, [blue_number](card const x) {
			//                                 ^ capture-by-value
			return x == blue_number;
		}));
	}

	SECTION("Closures (named lambda expressions)") {
		auto const blue_then_yellow = [](card const x, card const y) {
			return x.colour == colour::blue and y.colour == colour::yellow;
		};

		auto const blue_card = ranges::adjacent_find(hand, blue_then_yellow);
		REQUIRE(blue_card != hand.end());
		CHECK(*blue_card == comp6771::blue_skip);

		auto const yellow_card = ranges::next(blue_card);
		CHECK(*yellow_card == comp6771::yellow_draw_four);
	}

	SECTION("Lambda with capture-by-reference (with house rules)") {
		// house rule: two players can swap a card of the same value
		// (but for a different colour)
		auto cards_swapped = std::map<card, int>{};
		ranges::transform(hand, hand.begin(), [&cards_swapped](card const c) {
			//                                  ^ capture-by-reference
			if (c.colour != colour::blue) {
				return c;
			}

			note_swaps(cards_swapped, c);
			return card{colour::green, c.value};
		});

		CHECK(ranges::none_of(hand, [](card const c) { return c.colour == colour::blue; }));

		{
			using comp6771::blue_number;
			REQUIRE(cards_swapped.contains(blue_number));
			CHECK(cards_swapped.at(blue_number) == 4);
			auto const green_number = card{colour::green, comp6771::value::number};
			CHECK(ranges::count(hand, green_number) == 4);
		}
		{
			using comp6771::blue_skip;
			REQUIRE(cards_swapped.contains(blue_skip));
			CHECK(cards_swapped.at(blue_skip) == 2);
			auto const green_skip = card{colour::green, comp6771::value::skip};
			CHECK(ranges::count(hand, green_skip) == 2);
		}
	}
}
