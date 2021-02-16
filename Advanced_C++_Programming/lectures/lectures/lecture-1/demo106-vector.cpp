// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <vector>

#include "catch2/catch.hpp"
#include "range/v3/algorithm.hpp"
#include "range/v3/range.hpp"

TEST_CASE("Collections of objects") {
	auto const single_digits = std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto more_single_digits = single_digits;
	REQUIRE(single_digits == more_single_digits);

	more_single_digits[2] = 0;
	CHECK(single_digits != more_single_digits);

	more_single_digits.push_back(0);
	CHECK(ranges::count(more_single_digits, 0) == 3); // we'll explore algorithms in more detail
	                                                  // later

	more_single_digits.pop_back();
	CHECK(ranges::count(more_single_digits, 0) == 2);

	CHECK(std::erase(more_single_digits, 0) == 2);
	CHECK(ranges::count(more_single_digits, 0) == 0);
	CHECK(ranges::distance(more_single_digits) == 8);
}
