#include <catch2/catch.hpp>
#include <string>
#include <vector>

auto int_to_string(std::vector<int> const&) -> std::string;

TEST_CASE("int_to_string converts correctly") {}
