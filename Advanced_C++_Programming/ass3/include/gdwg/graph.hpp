#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <cassert>
#include <concepts/concepts.hpp>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/algorithm/transform.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/iterator/insert_iterators.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/utility.hpp>
#include <range/v3/view.hpp>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace gdwg {

	template<concepts::regular N, concepts::regular E>
	requires concepts::totally_ordered<N>and concepts::totally_ordered<E> //
	   class graph {
	private:
		using node_ptr = std::shared_ptr<N>;
		using weight_ptr = std::shared_ptr<E>;
		struct node_compare {
			using is_transparent = void;
			auto operator()(const node_ptr& a, const node_ptr& b) const -> bool {
				return *a < *b;
			}

			auto operator()(const node_ptr& a, const N b) const -> bool {
				return *a < b;
			}

			auto operator()(const N a, const node_ptr& b) const -> bool {
				return a < *b;
			}
		};

		struct weight_compare {
			using is_transparent = void;
			auto operator()(const weight_ptr& a, const weight_ptr& b) const -> bool {
				return *a < *b;
			}

			auto operator()(const weight_ptr& a, const E b) const -> bool {
				return *a < b;
			}

			auto operator()(const E a, const weight_ptr& b) const -> bool {
				return a < *b;
			}
		};

		struct edge_compare {
			using is_transparent = void;
			auto operator()(const std::pair<node_ptr, weight_ptr>& a,
			                const std::pair<node_ptr, weight_ptr>& b) const -> bool {
				if (*(a.first) == *(b.first)) {
					return *(a.second) < *(b.second);
				}
				return *(a.first) < *(b.first);
			}

			auto operator()(const std::pair<N, E>& a, const std::pair<node_ptr, weight_ptr>& b) const
			   -> bool {
				if (a.first == *(b.first)) {
					return a.second < *(b.second);
				}
				return a.first < *(b.first);
			}

			auto operator()(const std::pair<node_ptr, weight_ptr>& a, const std::pair<N, E>& b) const
			   -> bool {
				if (*(a.first) == b.first) {
					return *(a.second) < b.second;
				}
				return *(a.first) < b.first;
			}
		};

		using edge = std::pair<node_ptr, weight_ptr>;
		using edge_set = std::set<edge, edge_compare>;
		using node_set = std::set<node_ptr, node_compare>;
		using weight_set = std::set<weight_ptr, weight_compare>;
		using node_map = std::map<node_ptr, edge_set, node_compare>;
		node_set node_set_;
		weight_set weight_set_;
		node_map node_map_;

	public:
		class iterator {
		public:
			using value_type = ranges::common_tuple<N, N, E>;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// Iterator constructor
			iterator() = default;

			// Iterator source
			auto operator*() const -> ranges::common_tuple<N const&, N const&, E const&> {
				return ranges::common_tuple<N const&, N const&, E const&>(*(this->outer_->first),
				                                                          *(this->inner_->first),
				                                                          *(this->inner_->second));
			}

			// Iterator traversal
			auto operator++() -> iterator& {
				++this->inner_;
				while (this->inner_ == this->outer_->second.end()) {
					++this->outer_;
					if (this->outer_ == this->end_) {
						this->inner_ = inner_iterator{};
						return *this;
					}
					this->inner_ = this->outer_->second.begin();
				}
				return *this;
			}

			auto operator++(int) -> iterator {
				auto copy = *this;
				++*this;
				return copy;
			}

			auto operator--() -> iterator& {
				if (this->inner_ != inner_iterator{} && this->inner_ != this->outer_->second.begin()) {
					--this->inner_;
					return *this;
				}

				// i did not handle the case begin() - 1, the user should handle it
				do {
					--this->outer_;
				} while (this->outer_->second.empty());

				this->inner_ = --(this->outer_->second.end());
				return *this;
			}

			auto operator--(int) -> iterator {
				auto copy = *this;
				--*this;
				return copy;
			}

			// Iterator comparison
			auto operator==(iterator const& other) const -> bool {
				return this->end_ == other.end_ && this->inner_ == other.inner_
				       && this->outer_ == other.outer_;
			}

		private:
			using outer_iterator = typename node_map::const_iterator;
			using inner_iterator = typename edge_set::const_iterator;
			outer_iterator end_;
			outer_iterator outer_;
			inner_iterator inner_;

			explicit iterator(outer_iterator end, outer_iterator outer, inner_iterator inner) noexcept
			: end_(end)
			, outer_(outer)
			, inner_(inner) {}

			friend class graph;
		};
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// value initialises all members
		graph()
		: node_set_({})
		, weight_set_({})
		, node_map_({}) {}

		graph(std::initializer_list<N> il) noexcept
		: graph(il.begin(), il.end()) {}

		template<ranges::forward_iterator I, ranges::sentinel_for<I> S>
		requires ranges::indirectly_copyable<I, N*> //
		// Initialises the graph's node collection with the range [first, last)
		graph(I first, S last) noexcept
		: graph() {
			for (auto i = first; i != last; ++i) {
				auto result_pair = this->node_set_.insert(std::make_shared<N>(*i));
				if (get<1>(result_pair)) {
					this->node_map_.insert(
					   std::pair<node_ptr, edge_set>(*(get<0>(result_pair)), edge_set{}));
				}
			}
		}

		template<ranges::forward_iterator I, ranges::sentinel_for<I> S>
		requires ranges::indirectly_copyable<I, value_type*> //
		// Initialises the graph's node and edge collections with the range [first, last).
		graph(I first, S last) noexcept
		: graph() {
			for (auto i = first; i != last; ++i) {
				this->insert_node(i->from);
				this->insert_node(i->to);
				this->insert_edge(i->from, i->to, i->weight);
			}
		}

		// move assignment
		graph(graph&& other) noexcept = default;

		// copy construction
		graph(graph const& other) noexcept
		: graph() {
			ranges::transform(other.node_set_,
			                  ranges::inserter(this->node_set_, this->node_set_.begin()),
			                  [](node_ptr node) -> node_ptr { return std::make_shared<N>(*node); });
			ranges::transform(
			   other.weight_set_,
			   ranges::inserter(this->weight_set_, this->weight_set_.begin()),
			   [](weight_ptr weight) -> weight_ptr { return std::make_shared<E>(*weight); });
			ranges::transform(other.node_map_,
			                  ranges::inserter(this->node_map_, this->node_map_.begin()),
			                  [this](auto& pair) {
				                  auto key = this->node_set_.find(*(pair.first));
				                  auto edges = edge_set{};
				                  ranges::transform(pair.second,
				                                    ranges::inserter(edges, edges.begin()),
				                                    [this](auto& edge) {
					                                    return std::make_pair(
					                                       *(this->node_set_.find(edge.first)),
					                                       *(this->weight_set_.find(edge.second)));
				                                    });

				                  return std::make_pair(*key, edges);
			                  });
		}

		auto operator=(graph&& other) noexcept -> graph& {
			if (this == &other) { // learned from ass2 feedback, "what if this == &other?"
				return *this;
			}
			auto copy = graph(std::move(other));
			ranges::swap(this->node_set_, copy.node_set_);
			ranges::swap(this->weight_set_, copy.weight_set_);
			ranges::swap(this->node_map_, copy.node_map_);
			return *this;
		}

		auto operator=(graph const& other) noexcept -> graph& {
			auto copy = graph(other);
			ranges::swap(this->node_set_, copy.node_set_);
			ranges::swap(this->weight_set_, copy.weight_set_);
			ranges::swap(this->node_map_, copy.node_map_);
			return *this;
		}

		auto insert_node(N const& value) noexcept -> bool {
			if (this->node_set_.find(value) != this->node_set_.end()) {
				return false;
			}
			// pair<iterator,bool> insert ( const value_type& x );
			auto result_pair = this->node_set_.insert(std::make_shared<N>(value));
			// new node, also need to insert emepty set into node_map
			// help Extractor becuasue we need to also print out nodes with no edge
			this->node_map_.insert(std::pair<node_ptr, edge_set>(*(get<0>(result_pair)), edge_set{}));
			return true;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!(is_node(src)) || !(is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
			auto it = this->node_map_.find(src);
			assert(it != this->node_map_.end());
			this->weight_set_.insert(std::make_shared<E>(weight));
			auto new_edge =
			   std::make_pair(*(this->node_set_.find(dst)), *(this->weight_set_.find(weight)));
			return get<1>(it->second.insert(new_edge));
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			if (is_node(new_data)) {
				return false;
			}
			this->node_set_.erase(this->node_set_.find(old_data));
			auto new_data_it = get<0>(this->node_set_.insert(std::make_shared<N>(new_data)));
			for (auto& element : this->node_map_) {
				auto deleted_data = std::set<edge>{};
				std::erase_if(element.second,
				              [new_data_it, &old_data, &deleted_data](auto const& pair) -> bool {
					              if (*(pair.first) == old_data) {
						              deleted_data.insert(std::make_pair(*new_data_it, pair.second));
						              return true;
					              }
					              return false;
				              });
				element.second.insert(deleted_data.begin(), deleted_data.end());
			}
			auto old_data_map_it = this->node_map_.find(old_data);
			auto copy_set = old_data_map_it->second;
			this->node_map_.erase(old_data_map_it);
			this->node_map_.insert(std::make_pair(*new_data_it, copy_set));
			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!(is_node(old_data) || !(is_node(new_data)))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
			this->node_set_.erase(this->node_set_.find(old_data));
			auto new_data_it = this->node_set_.find(new_data);
			for (auto& element : this->node_map_) {
				auto deleted_data = std::set<edge>{};
				std::erase_if(element.second,
				              [new_data_it, &old_data, &deleted_data](auto const& pair) -> bool {
					              if (*(pair.first) == old_data) {
						              deleted_data.insert(std::make_pair(*new_data_it, pair.second));
						              return true;
					              }
					              return false;
				              });
				element.second.insert(deleted_data.begin(), deleted_data.end());
			}
			auto old_data_map_it = this->node_map_.find(old_data);
			auto copy_set = old_data_map_it->second;
			this->node_map_.erase(old_data_map_it);
			auto it = this->node_map_.find(new_data);
			it->second.insert(copy_set.begin(), copy_set.end());
		}
		// total time = 2 * log(N) + O(edge) + O(E) (N = # node, E = # weight)
		//            = O(log(N) + edge + E)
		auto erase_node(N const& value) noexcept -> bool {
			auto it_target_node = this->node_set_.find(value);
			if (it_target_node == this->node_set_.end()) {
				return false;
			}
			this->node_set_.erase(it_target_node); // time = O(log(n))
			this->node_map_.erase(this->node_map_.find(value)); // time = O(log(n))
			// time = O(edge) (not weight E)
			for (auto& element : this->node_map_) {
				std::erase_if(element.second,
				              [&value](auto const& pair) { return *(pair.first) == value; });
			}
			// clean up the unused weight
			// time = O(E) (weight)
			std::erase_if(this->weight_set_, [](auto const& weight) { return weight.use_count() == 1; });
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!(is_node(src)) || !(is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			auto w = this->weight_set_.find(weight);
			if (w == this->weight_set_.end()) {
				return false;
			}
			auto it = this->node_map_.find(src);
			auto it_target_pair = it->second.find(std::make_pair(dst, weight));
			if (it_target_pair == it->second.end()) {
				return false;
			}
			it->second.erase(it_target_pair);
			// clean up the unused weight
			if (w->use_count() == 1) {
				this->weight_set_.erase(w);
			}
			return true;
		}

		auto erase_edge(iterator i) -> iterator {
			auto weight = *(i.inner_->second);
			auto new_outer = i.outer_;
			auto no_const_new_outer = this->node_map_.erase(i.outer_, i.outer_);
			auto new_inner = no_const_new_outer->second.erase(i.inner_);
			if (new_inner == new_outer->second.end()) {
				++new_outer;
				new_outer = std::find_if(new_outer, i.end_, [](auto const& pair) -> bool {
					return !(pair.second.empty());
				});
				if (new_outer == i.end_) {
					new_inner = {};
				}
				else {
					new_inner = new_outer->second.begin();
				}
			}
			auto target_weight_it = this->weight_set_.find(weight);
			if (target_weight_it->use_count() == 1) {
				this->weight_set_.erase(target_weight_it);
			}
			return iterator{i.end_, new_outer, new_inner};
		}

		auto erase_edge(iterator i, iterator s) -> iterator {
			auto it = i;
			while (it != s) {
				it = this->erase_edge(it);
			}
			return it;
		}

		auto clear() noexcept -> void {
			this->node_map_.clear();
			this->node_set_.clear();
			this->weight_set_.clear();
		}

		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool {
			// set -> find O(logN) to search for an individual element
			return !(this->node_set_.find(value) == this->node_set_.end());
		}

		[[nodiscard]] auto empty() const noexcept -> bool {
			return this->node_set_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!(is_node(src) || !(is_node(dst)))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			auto const& target = this->node_map_.find(src)->second;
			return !(std::find_if(target.begin(),
			                      target.end(),
			                      [dst](const edge& p) -> bool { return *(p.first) == dst; })
			         == target.end());
		}

		[[nodiscard]] auto nodes() const noexcept -> std::vector<N> {
			auto result = std::vector<N>{};
			result.reserve(this->node_set_.size());
			std::transform(this->node_set_.begin(),
			               this->node_set_.end(),
			               std::back_inserter(result),
			               [](auto const node) { return *node; });
			return result;
		}

		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			if (!(is_node(src) || !(is_node(dst)))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			auto result = std::vector<E>{};
			auto const& target = this->node_map_.find(src)->second;
			for (auto const& edge : target) {
				if (*(edge.first) == dst) {
					result.push_back(*(edge.second));
				}
			}
			return result;
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (!(is_node(src))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			auto result = std::vector<N>{};
			auto const& target = this->node_map_.find(src)->second;
			std::transform(target.begin(), target.end(), std::back_inserter(result), [](auto const edge) {
				return *(edge.first);
			});
			result.erase(std::unique(result.begin(), result.end()), result.end());
			return result;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			auto outer = this->node_map_.find(src);
			if (outer == this->node_map_.end()) {
				return iterator{this->node_map_.end(), outer, {}};
			}
			auto inner = outer->second.find(std::make_pair(dst, weight));
			if (inner == outer->second.end()) {
				return iterator{this->node_map_.end(), this->node_map_.end(), {}};
			}
			return iterator{this->node_map_.end(), outer, inner};
		}

		// Complexity: O(n + e) n = # of node, e = # of edge
		[[nodiscard]] auto operator==(graph const& other) const noexcept -> bool {
			return this->node_set_.size() == other.node_set_.size()
			       && this->weight_set_.size() == other.weight_set_.size()
			       && this->node_map_.size() == other.node_map_.size()
			       && std::equal(this->node_set_.begin(),
			                     this->node_set_.end(),
			                     other.node_set_.begin(),
			                     [](auto const& a, auto const& b) { return *a == *b; })
			       && std::equal(this->weight_set_.begin(),
			                     this->weight_set_.end(),
			                     other.weight_set_.begin(),
			                     [](auto const& a, auto const& b) { return *a == *b; })
			       && std::equal(this->node_map_.begin(),
			                     this->node_map_.end(),
			                     other.node_map_.begin(),
			                     [](auto const& a, auto const& b) {
				                     return *(a.first) == *(b.first)
				                            && a.second.size() == b.second.size()
				                            && std::equal(a.second.begin(),
				                                          a.second.end(),
				                                          b.second.begin(),
				                                          [](auto const& a, auto const& b) {
					                                          return *(a.first) == *(b.first)
					                                                 && *(a.second) == *(b.second);
				                                          });
			                     });
		}

		// test helper
		[[nodiscard]] auto get_node_ref_count() const noexcept -> std::map<N, int> {
			auto result = std::map<N, int>{};
			ranges::transform(this->node_set_,
			                  ranges::inserter(result, result.begin()),
			                  [](auto const& node) { return std::make_pair(*node, node.use_count()); });
			return result;
		}

		// test helper
		[[nodiscard]] auto get_weight_ref_count() const noexcept -> std::map<E, int> {
			auto result = std::map<E, int>{};
			ranges::transform(
			   this->weight_set_,
			   ranges::inserter(result, result.begin()),
			   [](auto const& weight) { return std::make_pair(*weight, weight.use_count()); });
			return result;
		}

		[[nodiscard]] auto begin() const -> iterator {
			auto outer = std::find_if(this->node_map_.begin(),
			                          this->node_map_.end(),
			                          [](auto const& pair) { return !(pair.second.empty()); });
			if (outer == this->node_map_.end()) {
				return this->end();
			}
			auto a = *outer;
			return iterator{this->node_map_.end(), outer, outer->second.begin()};
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator{this->node_map_.end(), this->node_map_.end(), {}};
		}

		friend auto operator<<(std::ostream& os, graph const& g) noexcept -> std::ostream& {
			// might not able to use std::copy/std::ostream_iterator?
			for (auto const& element : g.node_map_) {
				os << *(element.first) << " (\n";
				for (auto const& edge : element.second) {
					os << "  " << *(edge.first) << " | " << *(edge.second) << "\n";
				}
				os << ")\n";
			}
			return os;
		}
	}; // namespace gdwg

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
