#include <catch2/catch.hpp>

#include "../../source/q1/q1.hpp"

TEST_CASE("basic test") {
	CHECK(q1::result() == 1);
}
