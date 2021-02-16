// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/range.hpp>
#include <vector>

TEST_CASE("Week 1 std::vector") {
	auto some_ints = std::vector<int>{0, 1, 2, 3, 2, 5};
	REQUIRE(ranges::distance(some_ints) == 6);

	SECTION("Querying a vector") {
		CHECK(some_ints[0] == 0);
		CHECK(some_ints[1] == 1);
		CHECK(some_ints[2] == 2);
		CHECK(some_ints[3] == 3);
		CHECK(some_ints[4] == 2);
		CHECK(some_ints[5] == 5);
	}

	SECTION("std::vector grows as we add elements") {
		some_ints.push_back(42);
		REQUIRE(ranges::distance(some_ints) == 7);

		CHECK(some_ints[6] == 42);
		CHECK(some_ints.back() == some_ints[6]);
	}

	SECTION("std::vector shrinks as we remove elements") {
		some_ints.pop_back();
		REQUIRE(ranges::distance(some_ints) == 5);

		CHECK(some_ints.back() == 2);

		CHECK(std::erase(some_ints, 2) == 2);
		REQUIRE(ranges::distance(some_ints) == 3);
		CHECK(some_ints == std::vector<int>{0, 1, 3});

		some_ints.clear(); // removes *all* the elements
		CHECK(some_ints.empty());

		auto const no_elements = std::vector<int>();
		REQUIRE(some_ints == no_elements);
	}
}

TEST_CASE("I want a vector with five zeroes") {
	auto const all_default = std::vector<double>(5);
	REQUIRE(ranges::distance(all_default) == 5);

	CHECK(all_default[0] == 0.0);
	CHECK(all_default[1] == 0.0);
	CHECK(all_default[2] == 0.0);
	CHECK(all_default[3] == 0.0);
	CHECK(all_default[4] == 0.0);
}

TEST_CASE("I want a vector with three identical values") {
	auto const initial_value = std::string("some words go here!");
	auto all_same = std::vector<std::string>(3, initial_value);
	REQUIRE(ranges::distance(all_same) == 3);

	CHECK(all_same[0] == initial_value);
	CHECK(all_same[1] == initial_value);
	CHECK(all_same[2] == initial_value);

	all_same[1] = "other words!";
	CHECK(all_same[0] != all_same[1]);
	CHECK(all_same.front() == all_same.back());
}
