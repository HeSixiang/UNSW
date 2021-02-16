// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <absl/container/flat_hash_set.h>
#include <catch2/catch.hpp>
#include <range/v3/range.hpp>
#include <string>

TEST_CASE("Sets of values") {
	auto computer_scientists = absl::flat_hash_set<std::string>{
	   "Lovelace",
	   "Babbage",
	   "Turing",
	   "Hamilton",
	   "Church",
	   "Borg",
	};

	REQUIRE(ranges::distance(computer_scientists) == 6);
	CHECK(computer_scientists.contains("Lovelace"));
	CHECK(not computer_scientists.contains("Gauss"));

	SECTION("Inserting and removing an element") {
		computer_scientists.insert("Gauss");
		CHECK(ranges::distance(computer_scientists) == 7);
		CHECK(computer_scientists.contains("Gauss"));

		computer_scientists.erase("Gauss");
		CHECK(ranges::distance(computer_scientists) == 6);
		CHECK(not computer_scientists.contains("Gauss"));
	}

	SECTION("Finding an element") {
		auto ada = computer_scientists.find("Lovelace");
		REQUIRE(ada != computer_scientists.end());

		CHECK(*ada == "Lovelace");
	}

	SECTION("An empty set") {
		computer_scientists.clear();
		CHECK(computer_scientists.empty());

		auto const no_names = absl::flat_hash_set<std::string>{};
		REQUIRE(no_names.empty());

		CHECK(computer_scientists == no_names);
	}
}
