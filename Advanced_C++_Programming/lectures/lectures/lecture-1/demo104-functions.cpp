// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "catch2/catch.hpp"

// Nullary function
auto is_about_cxx() -> bool {
	return true;
}

// Unary function
auto square(int const x) -> int {
	return x * x;
}

auto square(double const x) -> double {
	return x * x;
}

// Binary function
auto rectangular_area(double const width, double const length) -> double {
	return width * length;
}

TEST_CASE("Functions") {
	CHECK(is_about_cxx());
	CHECK(square(2) == 4);
	CHECK(square(2.0) == 4.0);
	CHECK(rectangular_area(2.0, 4.0) == 8.0);
}
