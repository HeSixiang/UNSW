#include <catch2/catch.hpp>

#include "../../source/q3/q3.hpp"

TEST_CASE("basic test") {
	CHECK(q3::result() == 3);
}
