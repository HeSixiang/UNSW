#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Utility function") {
	// checking all utility function
	comp6771::euclidean_vector v1{1.0, 2.0, 3.0};
	comp6771::euclidean_vector v2{4.0, -5.0, 6.0};
	comp6771::euclidean_vector exception(0);
	comp6771::euclidean_vector exception2(5);
	SECTION("norm") {
		auto result1 = comp6771::euclidean_norm(v1);
		auto expect_result1 = sqrt(1 + 4 + 9);
		CHECK(result1 == Approx(expect_result1));
		auto result2 = comp6771::euclidean_norm(v2);
		auto expect_result2 = sqrt(pow(4.0, 2) + pow(-5.0, 2) + pow(6.0, 2));
		CHECK(result2 == Approx(expect_result2));
		// check caching works correctly
		result1 = comp6771::euclidean_norm(v1);
		CHECK(result1 == Approx(expect_result1));
		result2 = comp6771::euclidean_norm(v2);
		CHECK(result2 == Approx(expect_result2));
		CHECK_THROWS_MATCHES(comp6771::euclidean_norm(exception),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with no dimensions does not "
		                                              "have a norm"));
	}

	SECTION("unit") {
		auto result1 = comp6771::unit(v1);
		auto expect_result1 =
		   comp6771::euclidean_vector{1.0 / sqrt(14), sqrt(2.0 / 7.0), 3.0 / sqrt(14)};
		auto result2 = comp6771::unit(v2);
		auto expect_result2 = comp6771::euclidean_vector{4.0 / sqrt(77), -5.0 / sqrt(77), 6 / sqrt(77)};
		CHECK(result1 == expect_result1);
		CHECK(result2 == expect_result2);
		CHECK_THROWS_MATCHES(comp6771::unit(exception),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with no dimensions does not "
		                                              "have a unit vector"));

		CHECK_THROWS_MATCHES(comp6771::unit(exception2),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with zero euclidean normal "
		                                              "does not have a unit vector"));
	}

	SECTION("dot") {
		auto result = comp6771::dot(v1, v2);
		auto expect_result = 12.0;
		CHECK(result == Approx(expect_result));
		CHECK_THROWS_MATCHES(comp6771::dot(v1, exception),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(3) and RHS(0) do not "
		                                              "match"));
	}
}
