// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "catch2/catch.hpp"
#include "gsl-lite/gsl-lite.hpp"

TEST_CASE("Promoting conversions") {
	auto const i = 42;

	SECTION("Promoting conversions") { // These are okay, but prefer explicit conversions below
		auto d = 0.0;
		d = i; // Silent conversion from int to double
		CHECK(d == 42.0);
		CHECK(d != 41);
	}

	SECTION("Explicit conversions") { // Preferred over implicit, since your intention is clear
		auto const d = static_cast<double>(i);
		CHECK(d == 42.0);
		CHECK(d != 41);
	}
}

TEST_CASE("Narrowing (lossy) conversions") {
	auto const i = 42;

	SECTION("Explicit conversions") { // information still lost, but we're saying we know
		auto const b = gsl_lite::narrow_cast<bool>(i);
		CHECK(b == true);
		CHECK(b == gsl_lite::narrow_cast<bool>(42)); // okay
	}
}
