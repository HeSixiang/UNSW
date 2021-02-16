#include <absl/container/flat_hash_set.h>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include "all_unique.hpp"

// Note that the intention for this tutorial question is to give students an easy introduction into
// `absl::flat_hash_set`. There might be other ways to solve this, but you'll need to use
// `absl::flat_hash_set` to solve Word Ladder, so it's best if there's a quick way for you to get
// some hands-on experience.
auto all_unique(std::string const& word) -> bool {
	// `absl::flat_hash_set` is a data structure that contains exactly zero or one element for a
	// given hash. Since we're only looking to detect if we've already inserted a letter, we can bail
	// whenever an insertion fails.
	auto letters_seen = absl::flat_hash_set<char>();

	for (auto const letter : word) {
		// Many containers' member functions return useful information under certain circumstances.
		// In the case of `absl::flat_hash_set::insert` and `absl::flat_hash_set::emplace`, we get
		// back both the iterator to the element and a flag that tells us whether or not the insertion
		// happened this time around (`true`), or if it was already inserted (`false`). If the value
		// wasn't inserted, then that means it was already present (and in our case, that means it's
		// been seen before).
		if (not letters_seen.emplace(letter).second) {
			return false;
		}
	}

	return true;
}

// You're encouraged to be a bit more creative with this implementation of `all_unique`; we use
// algorithms and views to solve this, since they're very handy for Word Ladder as well.
// Since we intend to modify `word` we pass it by value so that we don't need to have a `word_copy`
// later on.
auto all_unique_in_place1(std::string word) -> bool {
	ranges::sort(word);

	// adjacent_find compares each element to the element on its right (except for the last one,
	// which it skips), and applies a binary predicate with these elements. By default, adjacent_find
	// will apply ranges::equal_to, which means that adjacent_find will return an iterator to the
	// first duplicate that it finds. Since we only care about whether it's found a duplicate, we
	// only need to inspect if it's reached the end of the string.
	return ranges::adjacent_find(word) == word.cend();
}

// This solution might feel a little foreign if you're not used to pipelines; if you've used a
// Unix shell or PowerShell before, then you should hopefully have been introduced to pipelines
// before. This solution would be roughly equivalent to the Unix command `sort | uniq -c` if we
// pretend each character is on its own line.
//
// Note: since views::unique wasn't in the lectures, it's okay if you didn't think of this solution.
// Its inclusion is to get you thinking about how range pipelines have similarities to shell
// pipelines.
auto all_unique_in_place2(std::string word) -> bool {
	ranges::sort(word);
	namespace views = ranges::views;
	return ranges::distance(word | views::unique) == ranges::distance(word);
}
