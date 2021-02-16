#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

TEST_CASE("friends check") {
	// checking all friend functions
	auto v1 = comp6771::euclidean_vector{1.1, 2.2, 3.3, 4.4, 5.5};
	auto v2 = comp6771::euclidean_vector{1.1, 2.2, 3.3, 4.4, 5.5};
	comp6771::euclidean_vector v_zero_1(0);
	comp6771::euclidean_vector v_zero_2(0);
	auto v3 = v1;
	// use referenece to change the value
	auto& change_index = v3.at(0);
	change_index = 1.0;

	double factor{2.11};

	// used to check exception
	auto exception = comp6771::euclidean_vector();
	double exception_factor{0.0};
	SECTION("Equal") {
		CHECK(v1 == v1);
		CHECK(v1 == v2);
		CHECK(!(v1 == v3));
	}

	SECTION("Not equal") {
		CHECK(!(v1 != v1));
		CHECK(!(v1 != v2));
		CHECK(v1 != v3);
	}

	SECTION("Addition") {
		auto v4 = v1 + v3;
		auto v5 = v1 + v1;
		for (auto i = 0; i < v4.dimensions(); ++i) {
			CHECK(v4.at(i) == v1.at(i) + v3.at(i));
		}
		for (auto i = 0; i < v5.dimensions(); ++i) {
			CHECK(v5.at(i) == v1.at(i) + v1.at(i));
		}
		// check result of two 0-dimension vectors
		v4 = v_zero_1 + v_zero_2;
		CHECK(v4.dimensions() == 0);
		CHECK(fmt::format("{}", v4) == "[]");

		CHECK_THROWS_MATCHES((v5 = v1 + exception),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(5) and RHS(1) do not "
		                                              "match"));
	}

	SECTION("Substraction") {
		auto v4 = v1 - v3;
		auto v5 = v1 - v2;
		for (auto i = 0; i < v4.dimensions(); ++i) {
			CHECK(v4.at(i) == v1.at(i) - v3.at(i));
		}
		for (auto i = 0; i < v5.dimensions(); ++i) {
			CHECK(v5.at(i) == v1.at(i) - v2.at(i));
		}
		// check result of two 0-dimension vectors
		v4 = v_zero_1 - v_zero_2;
		CHECK(v4.dimensions() == 0);
		CHECK(fmt::format("{}", v4) == "[]");
		CHECK_THROWS_MATCHES((v5 = v1 - exception),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(5) and RHS(1) do not "
		                                              "match"));
	}

	SECTION("Multiply") {
		auto v4 = v1 * factor;
		auto v5 = factor * v1;
		for (auto i = 0; i < v4.dimensions(); ++i) {
			CHECK(v4.at(i) == v1.at(i) * factor);
		}

		for (auto i = 0; i < v4.dimensions(); ++i) {
			CHECK(v4.at(i) == factor * v1.at(i));
		}
		// check result of 0-dimension vector
		v4 = v_zero_1 * 0.0;
		CHECK(v4.dimensions() == 0);
		CHECK(fmt::format("{}", v4) == "[]");
		v4 = 0.0 * v_zero_1;
		CHECK(v4.dimensions() == 0);
		CHECK(fmt::format("{}", v4) == "[]");
	}

	SECTION("Divide") {
		auto v4 = v1 / factor;
		for (auto i = 0; i < v4.dimensions(); ++i) {
			CHECK(v4.at(i) == v1.at(i) / factor);
		}
		// check result of 0-dimension vector
		v4 = v_zero_1 / 1.1;
		CHECK(v4.dimensions() == 0);
		CHECK(fmt::format("{}", v4) == "[]");
		CHECK_THROWS_MATCHES(v4 = v1 / exception_factor,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Invalid vector division by 0"));
	}

	SECTION("Output Stream") {
		auto const a1 = comp6771::euclidean_vector(3, 3.0);
		auto const a2 = comp6771::euclidean_vector(3, 3.0);
		auto const a3 = comp6771::euclidean_vector(3, 3.0);

		CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]");
		CHECK(fmt::format("{}", comp6771::euclidean_vector(0)) == "[]");
	}
}
