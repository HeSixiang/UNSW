#include "comp6771/rope_q31.hpp"

#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/functional.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <utility>

static_assert(ranges::forward_iterator<rope::iterator>);
static_assert(ranges::forward_range<rope>);
static_assert(ranges::forward_range<rope const>);
static_assert(not ranges::output_iterator<rope::iterator, char>);

namespace views = ranges::views;

TEST_CASE("Default-constructed rope::iterator") {
	CHECK(rope::iterator{} == rope::iterator{});
}

TEST_CASE("Default-constructed rope") {
	auto r = rope();
	CHECK(r.begin() == r.end());
}

auto check_multi_pass(rope& r) -> void {
	{
		auto a = r.begin();
		auto b = r.begin();

		SECTION("Pre-increments are equal", "[iterator.concept.forward]") {
			REQUIRE(++a == ++b);
		}

		SECTION("Pre-increments are multi-pass", "[iterator.concept.forward]") {
			REQUIRE(([](rope::iterator x) { ++x; }(a), *a) == *b);
		}

		SECTION("Post-increment returns an object equal to its predecessor", "[iterator.concept.inc]") {
			CHECK(a++ == b);
		}

		SECTION("Post-increment still increments the object", "[iterator.concept.inc]") {
			CHECK(((void)a++, a) == ++b);
		}
	}
}

TEST_CASE("Single-element rope") {
	auto r = rope({"a"});
	{
		// Test for equality
		REQUIRE(r.begin() != r.end());
	}
	{
		REQUIRE(ranges::distance(r) == 1);
	}
	{ // value of element
		CHECK(*r.begin() == 'a');
	}

	check_multi_pass(r);
}

TEST_CASE("Single-string rope") {
	auto const word = std::string("abacus");
	auto r = rope({word});

	{
		// Test for equality
		REQUIRE(r.begin() != r.end());
	}
	{
		REQUIRE(ranges::distance(r) == ranges::distance(word));
	}
	{ // value of element
		CHECK(ranges::equal(r, word));
	}

	check_multi_pass(r);
}

TEST_CASE("Multi-string rope") {
	auto const sentence = std::string("the abacus is a kind of accumulator");
	auto to_string = [](auto r) { return r | ranges::to<std::string>; };
	auto words = sentence | views::split(' ') | views::transform(to_string) | ranges::to<std::vector>;
	REQUIRE(ranges::distance(words) == 7);

	auto r = rope(std::move(words));

	{
		// Test for equality
		REQUIRE(r.begin() != r.end());
	}

	auto is_space = [](char const c) { return c == ' '; };
	{
		auto const word_count = ranges::distance(sentence | views::remove_if(is_space));
		CHECK(ranges::distance(r) == word_count);
	}
	{ // value of element
		CHECK(ranges::equal(r, sentence | views::remove_if(is_space)));
	}

	check_multi_pass(r);
}
