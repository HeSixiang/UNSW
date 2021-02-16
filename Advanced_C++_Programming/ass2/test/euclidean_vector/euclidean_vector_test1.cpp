#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

TEST_CASE("basic test") {
	// Please note - this test may not function, it's just here for stubbing purposes
	auto const a1 = comp6771::euclidean_vector(3, 3.0);
	auto const a2 = comp6771::euclidean_vector(3, 3.0);
	auto const a3 = comp6771::euclidean_vector(3, 3.0);

	CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]");
}

// sample
TEST_CASE("Creation of unit vectors") {
	SECTION("You have two identical vectors") {
		auto a = comp6771::euclidean_vector(2);
		a[0] = 1;
		a[1] = 2;
		auto b = comp6771::euclidean_vector(2);
		b[0] = 1;
		b[1] = 2;

		auto c = comp6771::unit(a);
		auto d = comp6771::unit(b);
		REQUIRE(c == d);
	}
}
