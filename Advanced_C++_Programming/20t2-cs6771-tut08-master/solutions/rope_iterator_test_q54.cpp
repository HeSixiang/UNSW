#include "comp6771/rope_q33.hpp"

#include <catch2/catch.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/functional.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <utility>

static_assert(ranges::bidirectional_iterator<rope::iterator>);
static_assert(ranges::bidirectional_range<rope>);
static_assert(ranges::bidirectional_iterator<rope::const_iterator>);
static_assert(ranges::bidirectional_range<rope const>);
static_assert(ranges::output_iterator<rope::iterator, char>);

namespace views = ranges::views;

TEST_CASE("Default-constructed rope::iterator") {
	CHECK(rope::iterator{} == rope::iterator{});
	CHECK(rope::const_iterator{} == rope::iterator{});
	CHECK(rope::const_iterator{} == rope::const_iterator{});
}

TEST_CASE("Default-constructed rope") {
	auto r = rope();
	CHECK(r.begin() == r.end());

	auto const& cr = std::as_const(r);
	CHECK(cr.begin() == cr.end());
	CHECK(r.begin() == cr.begin());
	CHECK(cr.begin() == r.begin());
	CHECK(r.end() == cr.end());
	CHECK(cr.end() == r.end());
}

auto check_multi_pass(rope& r) -> void {
	{
		auto a = r.begin();
		auto b = r.begin();

		SECTION("Non-const pre-increments are equal", "[iterator.concept.forward]") {
			REQUIRE(++a == ++b);
		}
		SECTION("Const pre-increments are equal", "[iterator.concept.forward]") {
			REQUIRE(++rope::const_iterator(a) == ++rope::const_iterator(b));
		}

		SECTION("Non-const pre-increments are multi-pass", "[iterator.concept.forward]") {
			REQUIRE(([](rope::iterator x) { ++x; }(a), *a) == *b);
		}

		SECTION("Const pre-increments are multi-pass") {
			REQUIRE(([](rope::const_iterator x) { ++x; }(a), *a) == *b);
		}

		SECTION("Post-increment returns an object equal to its predecessor", "[iterator.concept.inc]") {
			CHECK(a++ == b);
		}

		SECTION("Post-increment still increments the object", "[iterator.concept.inc]") {
			CHECK(((void)a++, a) == ++b);
		}
	}
	{
		auto a = r.end();
		auto b = r.end();
		SECTION("Non-const pre-decrements are equal", "[iterator.concept.bidirectional]") {
			REQUIRE(--a == --b);
		}

		SECTION("Const pre-decrements are equal", "[iterator.concept.bidirectional]") {
			REQUIRE(--rope::const_iterator(a) == --rope::const_iterator(b));
		}

		if (ranges::distance(r.begin(), r.end()) == 1) {
			return;
		}

		--a;
		--b;
		SECTION("Non-const pre-decrements are multi-pass", "[iterator.concept.bidirectional]") {
			REQUIRE(([](rope::iterator x) { --x; }(a), *a) == *b);
		}

		SECTION("Const pre-decrements are multi-pass") {
			REQUIRE(([](rope::const_iterator x) { --x; }(a), *a) == *b);
		}

		SECTION("Post-decrement returns an object equal to its predecessor",
		        "[iterator.concept.bidirectional]") {
			CHECK(a-- == b);
		}

		SECTION("Post-decrement still decrements the object", "[iterator.concept.bidirectional]") {
			CHECK(((void)a--, a) == --b);
		}

		SECTION("Pre-decrements and pre-decrements can be undone by their inverse",
		        "[iterator.concept.bidirectional]") {
			CHECK(++(--a) == b);
			CHECK(--(++a) == b);
		}
	}
}

TEST_CASE("Single-element rope") {
	auto r = rope({"a"});
	auto const& cr = std::as_const(r);

	{
		// Test for equality
		REQUIRE(r.begin() != r.end());

		REQUIRE(cr.begin() != cr.end());
		REQUIRE(r.begin() == cr.begin());
		REQUIRE(cr.begin() == r.begin());
		REQUIRE(r.end() == cr.end());
		REQUIRE(cr.end() == r.end());
	}
	{
		REQUIRE(ranges::distance(r) == 1);
		REQUIRE(ranges::distance(cr) == 1);

		REQUIRE(ranges::distance(r | views::reverse) == 1);
		REQUIRE(ranges::distance(cr | views::reverse) == 1);
	}
	{ // value of element
		CHECK(*r.begin() == *cr.begin());
		CHECK(*r.begin() == 'a');
	}

	check_multi_pass(r);
}

TEST_CASE("Single-string rope") {
	auto const word = std::string("abacus");
	auto r = rope({word});
	auto const& cr = std::as_const(r);

	{
		// Test for equality
		REQUIRE(r.begin() != r.end());

		REQUIRE(cr.begin() != cr.end());
		REQUIRE(r.begin() == cr.begin());
		REQUIRE(cr.begin() == r.begin());
		REQUIRE(r.end() == cr.end());
		REQUIRE(cr.end() == r.end());
	}
	{
		REQUIRE(ranges::distance(r) == ranges::distance(word));
		REQUIRE(ranges::distance(cr) == ranges::distance(word));

		REQUIRE(ranges::distance(r | views::reverse) == ranges::distance(word));
		REQUIRE(ranges::distance(cr | views::reverse) == ranges::distance(word));
	}
	{ // value of element
		CHECK(ranges::equal(r, cr));
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
	auto const& cr = std::as_const(r);

	{
		// Test for equality
		REQUIRE(r.begin() != r.end());

		REQUIRE(cr.begin() != cr.end());
		REQUIRE(r.begin() == cr.begin());
		REQUIRE(cr.begin() == r.begin());
		REQUIRE(r.end() == cr.end());
		REQUIRE(cr.end() == r.end());
	}

	auto is_space = [](char const c) { return c == ' '; };
	{
		auto const word_count = ranges::distance(sentence | views::remove_if(is_space));
		CHECK(ranges::distance(r) == word_count);
		CHECK(ranges::distance(cr) == word_count);

		CHECK(ranges::distance(r | views::reverse) == word_count);
		CHECK(ranges::distance(cr | views::reverse) == word_count);
	}
	{ // value of element
		CHECK(ranges::equal(r, cr));
		CHECK(ranges::equal(r, sentence | views::remove_if(is_space)));
	}

	check_multi_pass(r);
}
