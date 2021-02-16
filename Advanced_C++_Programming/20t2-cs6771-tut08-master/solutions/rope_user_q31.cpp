#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include "comp6771/rope_q31.hpp"

static_assert(ranges::weakly_incrementable<rope::iterator>);
static_assert(ranges::indirectly_readable<rope::iterator>);
static_assert(ranges::input_iterator<rope::iterator>);
static_assert(ranges::forward_iterator<rope::iterator>);
static_assert(ranges::forward_range<rope>);
static_assert(ranges::forward_range<rope const>);

static_assert(not ranges::indirectly_writable<rope::iterator, char>);

int main() {
	auto const ropes = std::vector<rope>{
	   rope{{"a"}},
	   rope{{"abc"}},
	   rope{{"abc"}},
	   rope{{"abc", "def"}},
	   rope{{"abc", "", "def"}},
	   rope{{""}},
	   rope{{"", "abc", "def", ""}},
	};

	using namespace std::string_literals;
	ranges::transform(ropes, ranges::ostream_iterator(std::cout, "\n"), [](rope const& r) {
		return ranges::accumulate(r, "Rope: \""s) + '"';
	});
}
