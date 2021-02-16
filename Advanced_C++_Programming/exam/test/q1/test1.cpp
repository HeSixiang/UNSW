#include <fstream>
#include <sstream>

#include <catch2/catch.hpp>

#include "q1/q1.hpp"

// Please remember to run this file from the project directory
// ./build/test/q1/q1_test1

TEST_CASE("basic test") {
	auto ss = std::ostringstream{};
	q1::run_calculator("test/q1/in1.txt", ss);

	auto in = std::ifstream{};
	in.open("test/q1/out1.txt");
	auto buffer = std::ostringstream{};
	buffer << in.rdbuf();
	in.close();

	CHECK(buffer.str() == ss.str());
}