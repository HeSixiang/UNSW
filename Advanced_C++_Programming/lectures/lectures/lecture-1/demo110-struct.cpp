// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <string>
#include <vector>

#include "catch2/catch.hpp"

enum class field_of_study {
	astronomy,
	aviation,
	biology,
	chemistry,
	computer_science,
	conveyancing,
	criminal_law,
	ip_law,
	mathematics,
	physics,
	psychology,
	sociology,
	tax_law,
	theology,
};

struct scientist {
	std::string family_name;
	std::string given_name;
	field_of_study primary_field;
	std::vector<field_of_study> secondary_fields;

	auto operator==(scientist const&) const -> bool = default;
};

TEST_CASE("Data structures") {
	auto famous_physicist = scientist{.family_name = "Newton",
	                                  .given_name = "Isaac",
	                                  .primary_field = field_of_study::physics,
	                                  .secondary_fields = {field_of_study::mathematics,
	                                                       field_of_study::astronomy,
	                                                       field_of_study::theology}};
	auto famous_mathematician = scientist{.family_name = "Gauss",
	                                      .given_name = "Carl Friedrich",
	                                      .primary_field = field_of_study::mathematics,
	                                      .secondary_fields = {field_of_study::physics}};

	CHECK(famous_physicist.family_name != famous_mathematician.family_name);
	CHECK(famous_physicist.given_name != famous_mathematician.given_name);
	CHECK(famous_physicist.primary_field != famous_mathematician.primary_field);
	CHECK(famous_physicist.secondary_fields != famous_mathematician.secondary_fields);

	CHECK(famous_physicist != famous_mathematician);
}
