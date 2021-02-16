// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

TEST_CASE("Splitting ranges") {
	auto const sentence = std::string("the quick brown fox jumps over the lazy dog");

	namespace views = ranges::views;
	auto to_string = [](auto x) { return x | ranges::to<std::string>; };
	auto const words = sentence //
	                   | views::split(' ') //
	                   | views::transform(to_string) //
	                   | ranges::to<std::vector>;

	auto const expected =
	   std::vector<std::string>{"the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"};
	CHECK(words == expected);
}

TEST_CASE("Flattening a range of ranges into one") {
	auto const words =
	   std::vector<std::string>{"the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"};

	namespace views = ranges::views;

	// literal "join everything together"
	auto const sentence_without_spaces = words | views::join | ranges::to<std::string>;
	CHECK(sentence_without_spaces == "thequickbrownfoxjumpsoverthelazydog");

	// join everything together with something in-between (in our case, a single space).
	auto const sentence_with_spaces = words | views::join(' ') | ranges::to<std::string>;
	CHECK(sentence_with_spaces == "the quick brown fox jumps over the lazy dog");
}

TEST_CASE("Joining multiple ranges into a single one") {
	// This is called a using-directive. We *only* use these for importing user-defined literals
	// (UDLs), because that's the only way UDLs are usable. clang-tidy already knows which namespaces
	// are allowed, and the rest are blocked.
	using namespace std::string_literals;

	// The `s` at the end of `"the quick brown "` is a suffix that means `std::string`. In other
	// words, the next line is isomorphic to `auto const first = std::string("the quick brown ");`
	auto const first = "the quick brown "s;
	auto const second = "fox jumps over"s;
	auto const third = std::vector<std::string>{" the", "lazy", "dog"};

	namespace views = ranges::views;

	// `views::concat` is similar to `views::join`, except that it is used to join multiple range
	// objects, whereas `views::join` is used to join a range-of-ranges (`third` is an example of a
	// range-of-ranges).
	auto const sentence_with_spaces = views::concat(first, second, third | views::join(' '))
	                                  | ranges::to<std::string>;
	CHECK(sentence_with_spaces == "the quick brown fox jumps over the lazy dog");
}
