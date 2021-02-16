#include "comp6771/euclidean_vector.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Default Constructor") {
	// check default constructor,
	// the default dimension should be 1
	// the defualt value should be 0.0
	auto v1 = comp6771::euclidean_vector();
	CHECK(v1.dimensions() == 1);
	CHECK(v1.at(0) == 0.0);

	comp6771::euclidean_vector v2;
	CHECK(v2.dimensions() == 1);
	CHECK(v2.at(0) == 0.0);

	// craete on heap
	auto* v3 = new comp6771::euclidean_vector();
	CHECK(v3->dimensions() == 1);
	CHECK(v3->at(0) == 0.0);
	delete v3;
}

TEST_CASE("Single-argument Constructor") {
	// assume the integer input will always be non-negative.
	// check a constructor that takes single-argument
	auto v0 = comp6771::euclidean_vector(0);
	CHECK(v0.dimensions() == 0);

	auto v1 = comp6771::euclidean_vector(1);
	CHECK(v1.dimensions() == 1);
	CHECK(v1.at(0) == 0.0);

	auto v2 = comp6771::euclidean_vector(1000);
	CHECK(v2.dimensions() == 1000);
	for (auto i = 0; i < 1000; ++i) {
		CHECK(v2.at(i) == 0.0);
	}

	auto temp1{3};
	auto v3 = comp6771::euclidean_vector(temp1);
	CHECK(v3.dimensions() == temp1);
	CHECK(v3.at(0) == 0.0);

	auto temp2{1000};
	auto v4 = comp6771::euclidean_vector(temp2);
	CHECK(v4.dimensions() == 1000);
	for (auto i = 0; i < 1000; ++i) {
		CHECK(v4.at(i) == 0.0);
	}

	comp6771::euclidean_vector v5(1);
	CHECK(v5.dimensions() == 1);
	CHECK(v5.at(0) == 0.0);

	comp6771::euclidean_vector v6(1000);
	CHECK(v6.dimensions() == 1000);
	for (auto i = 0; i < 1000; ++i) {
		CHECK(v6.at(i) == 0.0);
	}
}

TEST_CASE("Constructor") {
	// test the constructor with two arguments
	SECTION("euclidean_vector(int, double)") {
		auto v1 = comp6771::euclidean_vector(2, 4.0);
		CHECK(v1.dimensions() == 2);
		CHECK(v1.at(0) == 4.0);
		CHECK(v1.at(1) == 4.0);

		auto temp_int = int{3};
		auto temp_double = double{3.24};
		auto v2 = comp6771::euclidean_vector(temp_int, temp_double);
		CHECK(v2.dimensions() == 3);
		CHECK(v2.at(0) == 3.24);
		CHECK(v2.at(1) == 3.24);
		CHECK(v2.at(2) == 3.24);
	}
	SECTION("euclidean_verctor(const_iterator, const_iterator)") {
		std::vector<double> v{1.1, 2.2, 3.3, 4.4, 5.5};
		auto v1 = comp6771::euclidean_vector(v.begin(), v.end());
		CHECK(v1.dimensions() == 5);
		CHECK(v1.at(0) == 1.1);
		CHECK(v1.at(1) == 2.2);
		CHECK(v1.at(2) == 3.3);
		CHECK(v1.at(3) == 4.4);
		CHECK(v1.at(4) == 5.5);
	}

	SECTION("euclidean_vector(initializer_list<double>)") {
		auto v1 = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		CHECK(v1.dimensions() == 3);
		CHECK(v1.at(0) == 1.0);
		CHECK(v1.at(1) == 2.0);
		CHECK(v1.at(2) == 3.0);
		// check empty list
		auto v2 = comp6771::euclidean_vector({});
		CHECK(v2.dimensions() == 0);
	}
}

TEST_CASE("Copy Constructor") {
	// check copy constructor
	auto v = comp6771::euclidean_vector(6, 6.6);
	auto v1 = comp6771::euclidean_vector(v);
	REQUIRE(v1.dimensions() == v.dimensions());
	for (auto i = 0; i < v.dimensions(); ++i) {
		CHECK(v1.at(i) == v.at(i));
	}
}

TEST_CASE("Move Constructor") {
	// check  move constructor
	auto v = comp6771::euclidean_vector(6, 6.6);
	auto v1 = comp6771::euclidean_vector(std::move(v));
	REQUIRE(v1.dimensions() == 6);
	for (auto i = 0; i < v1.dimensions(); ++i) {
		CHECK(v1.at(i) == 6.6);
	}
}
