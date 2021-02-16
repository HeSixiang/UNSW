// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <queue>
#include <stack>

#include "cards.hpp"

TEST_CASE("Stacks") {
	using comp6771::card;

	auto deck = std::stack<card>();
	REQUIRE(deck.empty());

	// Adding elements
	using comp6771::blue_number;
	using comp6771::green_draw_two;
	using comp6771::red_number;
	using comp6771::yellow_draw_four;

	deck.push(red_number);
	deck.push(green_draw_two);
	deck.push(green_draw_two);
	deck.push(yellow_draw_four);
	deck.push(blue_number);
	REQUIRE(deck.size() == 5);

	SECTION("Removing elements") {
		// For reasons that we'll cover in later weeks, we can't use the conventional `pop` operation
		// to get the top element, ever. Instead, we need to query the element, and then manually
		// remove it.
		CHECK(deck.top() == blue_number);
		deck.pop();
		CHECK(deck.top() == yellow_draw_four);
		deck.pop();
		CHECK(deck.top() == green_draw_two);
		deck.pop();
		CHECK(deck.top() == green_draw_two);
		deck.pop();
		CHECK(deck.top() == red_number);
		deck.pop();

		REQUIRE(deck.empty());
	}

	SECTION("Comparing two stacks") {
		auto const more_cards = deck;
		REQUIRE(more_cards == deck);

		deck.pop();
		CHECK(more_cards != deck);
	}
}

TEST_CASE("Queues") {
	auto computer_scientists = std::queue<std::string>();
	REQUIRE(computer_scientists.empty());

	computer_scientists.push("Ada");
	computer_scientists.push("Charles");
	computer_scientists.push("Alan");
	computer_scientists.push("Margaret");
	computer_scientists.push("Alonzo");
	computer_scientists.push("Anita");
	REQUIRE(computer_scientists.size() == 6);

	SECTION("Removing elements") {
		// For reasons that we'll cover in later weeks, we can't use the conventional `pop` operation
		// to get the top element, ever. Instead, we need to query the element, and then manually
		// remove it.
		CHECK(computer_scientists.front() == "Ada");
		computer_scientists.pop();
		CHECK(computer_scientists.front() == "Charles");
		computer_scientists.pop();
		CHECK(computer_scientists.front() == "Alan");
		computer_scientists.pop();
		CHECK(computer_scientists.front() == "Margaret");
		computer_scientists.pop();
		CHECK(computer_scientists.front() == "Alonzo");
		computer_scientists.pop();
		CHECK(computer_scientists.front() == "Anita");
		computer_scientists.pop();

		REQUIRE(computer_scientists.empty());
	}

	SECTION("Comparing two queues") {
		auto const more_computer_scientists = computer_scientists;
		REQUIRE(more_computer_scientists == computer_scientists);

		computer_scientists.pop();
		CHECK(more_computer_scientists != computer_scientists);
	}
}
