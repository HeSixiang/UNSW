#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Three basic member function check") {
	// checking all memeber function
	std::vector<double> expect{1.1, 2.2, 3.3, 4.4, 5.5};
	auto v = comp6771::euclidean_vector(expect.begin(), expect.end());
	SECTION("double at(int) const") {
		for (auto i = 0; i < 5; ++i) {
			auto const temp = v.at(i);
			CHECK(temp == expect.at(static_cast<size_t>(i)));
		}
	}

	SECTION("double& at(int) const") {
		auto v1 = comp6771::euclidean_vector(5);
		// checking wether we are able to change the value
		for (auto i = 0; i < 5; ++i) {
			auto& temp = v1.at(i);
			temp = expect.at(static_cast<size_t>(i));
		}
		for (auto i = 0; i < 5; ++i) {
			auto const temp = v.at(i);
			CHECK(temp == expect.at(static_cast<size_t>(i)));
		}
	}

	SECTION("int dimensions") {
		CHECK(static_cast<size_t>(v.dimensions()) == expect.size());
	}
}
