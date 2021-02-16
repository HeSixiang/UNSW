#include <iostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include "comp6771/rope.hpp"

// Uncomment these static_asserts to confirm if your rope::iterator models an iterator.

//////////////////////////////// 3.1 ////////////////////////////////
// Hints: https://slides.com/cs6771/comp6771-19t2-7-2-custom-iterators#/4/10
// Hints: https://slides.com/cs6771/comp6771-19t2-7-2-custom-iterators#/7/7
// static_assert(ranges::weakly_incrementable<rope::iterator>);
// static_assert(ranges::indirectly_readable<rope::iterator>);
// static_assert(ranges::input_iterator<rope::iterator>);
// static_assert(ranges::forward_iterator<rope::iterator>);
// static_assert(ranges::forward_range<rope>);
// static_assert(ranges::forward_range<rope const>); // turn off for 3.3; re-enable for 5

//////////////////////////////// 3.3 ////////////////////////////////
// Hints: https://slides.com/cs6771/comp6771-19t2-7-2-custom-iterators#/8/4
// static_assert(ranges::bidirectional_iterator<rope::iterator>);
// static_assert(ranges::bidirectional_range<rope>);
// static_assert(not ranges::range<rope const>); // for 3.3 only

// Hints: https://slides.com/cs6771/comp6771-19t2-7-2-custom-iterators#/12/2
// static_assert(ranges::indirectly_writable<rope::iterator, char>);
// static_assert(not ranges::indirectly_writable<rope::const_iterator, char>);

// Hints: https://slides.com/cs6771/comp6771-19t2-7-2-custom-iterators#/12/5
// static_assert(ranges::output_iterator<rope::iterator, char>);
// static_assert(ranges::output_range<rope, char>);

//////////////////////////////// 5.3 ////////////////////////////////
// static_assert(ranges::bidirectional_iterator<rope::const_iterator>);

int main() {
	auto ropes = std::vector<rope>{
	   rope{{"a"}},
	   rope{{"abc"}},
	   rope{{"abc"}},
	   rope{{"abc", "def"}},
	   rope{{"abc", "", "def"}},
	   rope{{""}},
	   rope{{"", "abc", "def", ""}},
	};

	using namespace std::string_literals;
	ranges::transform(ropes, ranges::ostream_iterator(std::cout, "\n"), [](rope& r) {
		return ranges::accumulate(r, "Rope: \""s) + '"';
	});

	ranges::transform(ropes | ranges::views::reverse,
	                  ranges::ostream_iterator(std::cout, "\n"),
	                  [](rope& r) { return ranges::accumulate(r, "Reverse rope: \""s) + '"'; });
}
