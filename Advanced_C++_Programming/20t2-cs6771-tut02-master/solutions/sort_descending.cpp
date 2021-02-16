#include <range/v3/algorithm.hpp>
#include <range/v3/functional.hpp>
#include <range/v3/range.hpp>
#include <vector>

#include "sort_descending.hpp"

auto sort_descending(std::vector<std::string>& numbers) -> void {
	ranges::sort(numbers, ranges::greater{});
}
