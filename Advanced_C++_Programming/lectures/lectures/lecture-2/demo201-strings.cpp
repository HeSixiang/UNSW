// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <absl/strings/str_cat.h>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <string>

TEST_CASE("String Processing") {
	auto const greeting_control = std::string("hello, world!");

	SECTION("Introducing user-defined literals") {
		using namespace std::string_literals;
		auto const greeting_with_udl = "hello, world!"s;
		CHECK(greeting_control == greeting_with_udl);
	}

	SECTION("String concatenation") {
		auto const greeting_concatenated = absl::StrCat("hello", ", ", "world", "!");
		CHECK(greeting_control == greeting_concatenated);
	}

	SECTION("String formatting") {
		auto const message = fmt::format("The meaning of life is {}", 42);
		CHECK(message == "The meaning of life is 42");

		auto const pi_message = fmt::format("pi has the value {}", 3.1415);
		CHECK(pi_message == "pi has the value 3.1415");
	}

	SECTION("Named parameters") {
		auto const expected = std::string("life=42, pi=3.1415");

		auto const ohno = fmt::format("life={}, pi={}", 3.1415, 42); // oh no!
		CHECK(ohno == expected);

		auto const yay =
		   fmt::format("life={life}, pi={pi}, pi={pi}", fmt::arg("pi", 3.1415), fmt::arg("life", 42));
		CHECK(yay == expected);
	}
}
