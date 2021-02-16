#include <sstream>

#include <catch2/catch.hpp>

#include "q2/q2.hpp"

TEST_CASE("basic test") {
	auto m = q2::sparse_matrix<int>{};
	auto out = std::ostringstream{};
	out << m;
	auto const expected_output = std::string_view("(1,1,0)");
	CHECK(out.str() == expected_output);
}