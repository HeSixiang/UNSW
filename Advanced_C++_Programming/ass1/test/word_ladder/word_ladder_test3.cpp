#include "comp6771/word_ladder.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "comp6771/testing/range/contain.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/is_sorted.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/range/primitives.hpp"

TEST_CASE("work -> play") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const from = std::string{"work"};
	auto const to = std::string{"play"};

	// check the assumptions
	REQUIRE(english_lexicon.contains(from));
	REQUIRE(english_lexicon.contains(to));
	REQUIRE(from != to);
	REQUIRE(ranges::size(from) == ranges::size(to));

	auto const ladders = word_ladder::generate(from, to, english_lexicon);

	SECTION("check all words in the path can be found at dic") {
		INFO("all words should appear in english.txt");
		for (auto const& path : ladders) {
			for (auto const& word : path) {
				CHECK(english_lexicon.contains(word));
			}
		}
	}

	SECTION("check the result sorted in lexicographical order") {
		INFO("the result should be sorted in lexicographical order");
		CHECK(ranges::is_sorted(ladders));
	}

	SECTION("check number of possible path") {
		INFO("the number of path should be 12");
		CHECK(ranges::size(ladders) == 12);
	}

	SECTION("check the size of each path") {
		INFO("the shortest path should be 7");
		for (auto const& path : ladders) {
			CHECK(ranges::size(path) == 7);
		}
	}

	SECTION("check there is only one char different between adjacent words") {
		for (auto const& path : ladders) {
			auto temp = std::adjacent_find(path.begin(),
			                               path.end(),
			                               [](std::string const& current, std::string const& next) {
				                               auto check = false;
				                               if (ranges::size(current) != ranges::size(next)) {
					                               return check;
				                               }
				                               for (std::string::size_type i = 0; i < current.size(); ++i)
				                               {
					                               if (current[i] != next[i]) {
						                               if (check) {
							                               return check;
						                               }
						                               check = true;
					                               }
				                               }
				                               return check;
			                               });
			CHECK(temp != path.end());
		}
	}

	SECTION("check correctness by using one possible solution") {
		CHECK(
		   ranges::any_of(ladders,
		                  testing::contain({"work", "fork", "form", "foam", "flam", "flay", "play"})));
	}

	SECTION("check correctness by using another direction") {
		auto reverse_ladders = word_ladder::generate(to, from, english_lexicon);
		for (auto& path : reverse_ladders) {
			std::reverse(path.begin(), path.end());
		}
		ranges::sort(reverse_ladders);
		CHECK(ladders == reverse_ladders);
	}
}
