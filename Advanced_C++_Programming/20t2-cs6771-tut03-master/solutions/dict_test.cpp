#include <istream>
#include <string>
#include <vector>
#include <sstream>

#include "catch2/catch.hpp"
#include "../source/dict.h"

//  program.
std::string get_valid_words(const std::vector<std::string>& words, const std::string& value) {
  std::istringstream is{value};
  std::ostringstream out;
  print_valid_words(words, is, out);
  return out.str();
}

// testing is enough, and discuss the value of each different require that you add.
TEST_CASE("Empty word list means nothing is valid") {
  REQUIRE(get_valid_words({}, "").empty());
  REQUIRE(get_valid_words({}, "hello").empty());
  REQUIRE(get_valid_words({}, "hello world").empty());
}

TEST_CASE("One word means only that word is valid") {
  REQUIRE(get_valid_words({"hello"}, "").empty());
  // TODO(tutorial): Add more tests here
}

TEST_CASE("Multiple words means they are all valid") {
  REQUIRE(get_valid_words({"hello", "world"}, "").empty());
  // TODO(tutorial): Add more tests here
}