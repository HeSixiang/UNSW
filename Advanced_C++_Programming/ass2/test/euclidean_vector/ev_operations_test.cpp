#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("Operations test") {
	comp6771::euclidean_vector v{1.1, 2.2, 3.3, 4.4, 5.5};
	SECTION("Copy Assignment") {
		comp6771::euclidean_vector v1;
		v1 = v;
		CHECK(v1.dimensions() == 5);
		CHECK(v1.at(0) == v.at(0));
		CHECK(v1.at(1) == v.at(1));
		CHECK(v1.at(2) == v.at(2));
		CHECK(v1.at(3) == v.at(3));
		CHECK(v1.at(4) == v.at(4));
	}

	SECTION("Move Assignment") {
		comp6771::euclidean_vector v1;
		v1 = ranges::move(v);
		CHECK(v1.dimensions() == 5);
		CHECK(v1.at(0) == v.at(0));
		CHECK(v1.at(1) == v.at(1));
		CHECK(v1.at(2) == v.at(2));
		CHECK(v1.at(3) == v.at(3));
		CHECK(v1.at(4) == v.at(4));
	}

	SECTION("Subscript") {
		// getter
		double const v1{v[2]};
		CHECK(v1 == 3.3);
		// setter
		v[2] = 3.0;
		CHECK(v.at(2) == 3.0);
	}

	SECTION("Unary plus") {
		CHECK(v.dimensions() == (+v).dimensions());
		CHECK(v.at(0) == (+v).at(0));
		CHECK(v.at(1) == (+v).at(1));
		CHECK(v.at(2) == (+v).at(2));
		CHECK(v.at(3) == (+v).at(3));
		CHECK(v.at(4) == (+v).at(4));
		// check unary plus is copy
		(+v)[0] = 0.0;
		CHECK(v.at(0) != 0.0);
	}

	SECTION("Negation") {
		comp6771::euclidean_vector const v1{-1.1, -2.2, -3.3, -4.4, -5.5};
		CHECK((-v).at(0) == v1.at(0));
		CHECK((-v).at(1) == v1.at(1));
		CHECK((-v).at(2) == v1.at(2));
		CHECK((-v).at(3) == v1.at(3));
		CHECK((-v).at(4) == v1.at(4));
		// check unary plus is copy
		(-v)[0] = 0.0;
		CHECK(v.at(0) != 0.0);
	}

	SECTION("Compound Addition") {
		comp6771::euclidean_vector const v1{0.1, 0.2, 0.3, 0.4, 0.5};
		v += v1;
		CHECK(v.at(0) == Approx(1.2));
		CHECK(v.at(1) == Approx(2.4));
		CHECK(v.at(2) == Approx(3.6));
		CHECK(v.at(3) == Approx(4.8));
		CHECK(v.at(4) == Approx(6.0));
		// check exception
		comp6771::euclidean_vector v2{0.1, 0.2, 0.3};
		CHECK_THROWS_MATCHES((v2 += v1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(3) and RHS(5) do not "
		                                              "match"));
	}

	SECTION("Compound Subtraction") {
		comp6771::euclidean_vector const v1{0.1, 0.2, 0.3, 0.4, 0.5};
		v -= v1;
		CHECK(v.at(0) == 1.0);
		CHECK(v.at(1) == 2.0);
		CHECK(v.at(2) == 3.0);
		CHECK(v.at(3) == 4.0);
		CHECK(v.at(4) == 5.0);
		comp6771::euclidean_vector v2{0.1, 0.2, 0.3};
		CHECK_THROWS_MATCHES(v2 -= v1,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(3) and RHS(5) do not "
		                                              "match"));
	}

	SECTION("Compound Multiplication") {
		double v1{0.1};
		v *= v1;
		CHECK(v.at(0) == Approx(0.11));
		CHECK(v.at(1) == Approx(0.22));
		CHECK(v.at(2) == Approx(0.33));
		CHECK(v.at(3) == Approx(0.44));
		CHECK(v.at(4) == Approx(0.55));
	}

	SECTION("Compound Division") {
		double v1{0.1};
		v /= v1;
		CHECK(v.at(0) == Approx(11.0));
		CHECK(v.at(1) == Approx(22.0));
		CHECK(v.at(2) == Approx(33.0));
		CHECK(v.at(3) == Approx(44.0));
		CHECK(v.at(4) == Approx(55.0));
		double v2{0.0};
		CHECK_THROWS_MATCHES(v /= v2,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Invalid vector division by 0"));
	}

	SECTION("Vector Type Conversion") {
		auto const expect = std::vector<double>{1.1, 2.2, 3.3, 4.4, 5.5};
		auto const v1 = static_cast<std::vector<double>>(v);
		CHECK(v1 == expect);
	}

	SECTION("List Type Conversion") {
		auto const expect = std::list<double>{1.1, 2.2, 3.3, 4.4, 5.5};
		auto const v1 = static_cast<std::list<double>>(v);
		CHECK(v1 == expect);
	}
}
