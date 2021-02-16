// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <catch2/catch.hpp>
#include <gsl-lite/gsl-lite.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <string>
#include <vector>

// Returns the minimum number of characters that need to be changed for two strings to be equal.
// This algorithm won't be very helpful for word-ladder, since it doesn't care whether or not the
// changes form a valid word.
auto hamming_distance(std::string const& s1, std::string const& s2) -> int {
	auto zip_different = ranges::views::zip_with(ranges::not_equal_to{}, s1, s2);
	return ranges::accumulate(zip_different, 0);
}

TEST_CASE("nary-transform") {
	namespace views = ranges::views;

	auto const x = views::iota(0, 5) | ranges::to<std::vector>;
	auto y = views::iota(5, 10);
	CHECK(ranges::equal(views::zip_with(ranges::plus{}, x, y), std::vector<int>{5, 7, 9, 11, 13}));

	auto z = views::iota(10, 15);
	auto xy_plus_z = [](int const x, int const y, int const z) { return x * y + z; };
	CHECK(ranges::equal(views::zip_with(xy_plus_z, x, y, z), std::vector<int>{10, 17, 26, 37, 50}));

	CHECK(::hamming_distance("chew", "chop") == 2);
	CHECK(::hamming_distance("hello", "world") == 4);
}

TEST_CASE("enumerate") {
	namespace views = ranges::views;

	auto const some_ints = views::iota(30, 40) | ranges::to<std::vector>;
	auto index_multiply = [](auto const value) {
		auto const [i, element] = value;
		return gsl_lite::narrow_cast<int>(i) * element;
	};

	auto product = views::enumerate(some_ints) | views::transform(index_multiply);

	auto const expected = std::vector<int>{0, 31, 64, 99, 136, 175, 216, 259, 304, 351};
	CHECK(ranges::equal(product, expected));
}
