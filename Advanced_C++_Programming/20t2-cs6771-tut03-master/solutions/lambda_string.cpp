// Author: Hayden
#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "range/v3/utility.hpp"

// There definitely may be better ways to do this...

TEST_CASE("Checks sort3(int&, int&, int&)") {
  std::vector<std::string> vec{"We", "love", "lambda", "functions"};

  auto const vowels = [] (char const& c) { return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'; };

  std::sort(vec.begin(), vec.end(), [vowels] (std::string const& x, std::string const& y) {
    auto const xcount = std::count_if(x.cbegin(), x.cend(), vowels);
    auto const ycount = std::count_if(y.cbegin(), y.cend(), vowels);
    if (xcount == ycount) {
      return (x.length() - static_cast<std::string::size_type>(xcount)) > (y.length() - static_cast<std::string::size_type>(ycount));
    }
    return xcount > ycount;
  });

  std::vector<std::string> correct_order{"functions", "lambda", "love", "We"};
  CHECK(correct_order == vec);
}