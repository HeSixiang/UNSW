#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "range/v3/utility.hpp"

TEST_CASE("Checks sort3(int&, int&, int&)") {
	std::vector<std::string> vec{"We", "love", "lambda", "functions"};

	// Sort it here

	std::vector<std::string> correct_order{"functions", "lambda", "love", "We"};
	CHECK(correct_order == vec);
}