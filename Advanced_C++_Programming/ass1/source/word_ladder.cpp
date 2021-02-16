// Sixiang He
// z5280561
#include "comp6771/word_ladder.hpp"

#include <range/v3/algorithm/sort.hpp>
#include <range/v3/view.hpp>
#include <queue>
#include <string>

namespace word_ladder {
	// this function will return the shortest path of one word to another as word_ladder
	// if there are many different shortest paths, it will return all of them
	auto generate(std::string const& from,
	              std::string const& to,
	              absl::flat_hash_set<std::string> const& lexicon)
	   -> std::vector<std::vector<std::string>> {
		// get the same length word from the lexicon and store into dic
		// because we only need to consider word, which has the same length as from and to
		auto intermedi = lexicon | ranges::views::filter([from](std::string const& word) {
			                 return from.length() == word.length();
		                 });
		auto dic = absl::flat_hash_set<std::string>{intermedi.begin(), intermedi.end()};

		// put the starting word into vector and store in the queue
		auto begin_path = std::vector<std::string>{from};
		auto helper_queue = std::queue<std::vector<std::string>>{};
		helper_queue.push(begin_path);

		// using visited set to record all the visited word at the same level
		auto visited_set = absl::flat_hash_set<std::string>{};

		// the final result will be stored here
		auto result = std::vector<std::vector<std::string>>{};

		// initialize the current depth and min depth
		std::vector<std::string>::size_type current_depth = 0;
		auto min_depth = current_depth;

		// trying to find the path until the queue is empty
		// if the queue is empty,
		// it means that we have already tried all possibles.
		while (not helper_queue.empty()) {
			// get the frst one in the queue
			auto current_path = helper_queue.front();
			helper_queue.pop();

			// if we go deeper(next level),
			// we need to remove all the visited word.
			auto current_path_depth = ranges::size(current_path);
			if (current_path_depth != current_depth) {
				current_depth = current_path_depth;
				for (auto const& visited_word : visited_set) {
					dic.erase(dic.find(visited_word));
				}
				visited_set.clear();
			}

			// check path is the answer or not.
			// if yes, then check wether there are other paths
			auto current_last_word = current_path.back();
			if (current_last_word == to) {
				if (min_depth == 0) {
					min_depth = current_path_depth;
				}
				if (min_depth != current_path_depth) {
					break;
				}
				result.push_back(current_path);
				continue; // no need to go deeper
			}

			// list all possible path
			for (auto& c_of_word : current_last_word) {
				auto orginal_char = c_of_word;
				// from a to z, put all possible
				for (auto c = 'a'; c <= 'z'; ++c) {
					if (c == orginal_char) {
						continue;
					}
					c_of_word = c;
					if (dic.find(current_last_word) == dic.end()) {
						continue;
					}
					auto new_path = current_path;
					new_path.push_back(current_last_word);
					helper_queue.push(new_path);
					visited_set.insert(current_last_word);
				}
				c_of_word = orginal_char; // change to orginal char
			}
		}
		ranges::sort(result); // sort to lexicographical order
		return result;
	}
} // namespace word_ladder
