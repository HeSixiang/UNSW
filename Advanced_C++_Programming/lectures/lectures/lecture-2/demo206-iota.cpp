// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <compare>
#include <vector>

TEST_CASE("views::iota --- generating a sequence of integers on demand") {
	namespace views = ranges::views; // namespace aliases allow us to shorthand a namespace

	// description: "I want to work with the first 100 integers."
	auto first_hundred = views::iota(0, 100);

	// action: I want the number of elements in the range.
	CHECK(ranges::distance(first_hundred) == 100);

	// action: I want the first element.
	CHECK(first_hundred.front() == 0);

	// action: I want the last element.
	CHECK(first_hundred.back() == 99);

	// action: I want all of the elements at once, as a vector.
	auto const all_at_once = first_hundred | ranges::to<std::vector>;

	// action: I want to generate each element one after the other, and compare it to the
	// corresponding value in another range.
	CHECK(ranges::equal(first_hundred, all_at_once));
}
