//
//  Copyright UNSW Sydney School of Computer Science and Engineering
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
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

TEST_CASE("at -> it") {
	auto const english_lexicon = word_ladder::read_lexicon("./english.txt");
	auto const from = std::string{"at"};
	auto const to = std::string{"it"};

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
		INFO("the number of path should be 1");
		CHECK(ranges::size(ladders) == 1);
	}

	SECTION("check the size of each path") {
		INFO("the shortest path should be 2");
		for (auto const& path : ladders) {
			CHECK(ranges::size(path) == 2);
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
		CHECK(ranges::any_of(ladders, testing::contain({from, to})));
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
