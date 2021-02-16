// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <string>

#include "catch2/catch.hpp"

TEST_CASE("Basic types") {
	SECTION("Numbers") {
		auto const meaning_of_life = 42; // type `int`, for integers.
		auto const six_feet_in_metres = 1.8288; // type `double`, for rational numbers.
		CHECK(six_feet_in_metres < meaning_of_life); // report if this expression is false
	}

	SECTION("Text") {
		auto const course_code = std::string("Applied Modern C++"); // type `string`, for text.
		auto const letter = 'A'; // type `char`, for single characters.
		CHECK(course_code.front() == letter);
	}

	SECTION("Truth values") {
		auto const is_cxx = true; // type `bool`, for truth
		auto const is_danish = false;
		CHECK(is_cxx != is_danish);
	}
}
