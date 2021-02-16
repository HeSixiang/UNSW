#include "q3/record.hpp"
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("test empty record serialisation") {
	auto const empty_record = "{\n}\n";
	auto r = q3::record{};
	auto ss = std::stringstream();
	ss << r;
	CHECK(ss.str() == empty_record);
}

TEST_CASE("test empty record deserialisation") {
	auto const empty_record = "{\n}\n";
	auto r = q3::record{};
	auto ss = std::stringstream(empty_record);
	ss >> r;
	CHECK(r.count() == 0);
}
