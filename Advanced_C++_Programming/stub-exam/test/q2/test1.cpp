#include <catch2/catch.hpp>

#include "../../source/q2/q2.hpp"

TEST_CASE("basic test") {
	CHECK(q2::result() == 2);
}
