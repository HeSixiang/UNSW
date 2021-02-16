#include <iostream>
#include <memory>
#include <optional>
#include <range/v3/iterator.hpp>
#include <range/v3/view.hpp>

namespace cs6771 {
	template<typename T>
	class list {
	public:
		class iterator;

		list() = default;

		list(std::initializer_list<T> il)
		: list(il.begin(), il.end()) {}

		template<ranges::input_iterator I, ranges::sentinel_for<I> S>
		list(I first, S last)
		: head_(make_nodes(first, last)) {
			// Bonus excercise: how can you improve this?
			if (head_ == nullptr) {
				return;
			}

			tail_ = head_.get();
			while (tail_->next != nullptr) {
				tail_ = tail_->next.get();
			}
		}

		[[nodiscard]] auto begin() const -> iterator {
			return iterator(head_.get());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator(tail_);
		}

	private:
		struct node {
			std::optional<T> value;
			std::unique_ptr<node> next;
			node* prev;
		};

		std::unique_ptr<node> head_ = nullptr;
		node* tail_ = nullptr;

		template<ranges::input_iterator I, ranges::sentinel_for<I> S>
		static auto make_nodes(I first, S last) -> std::unique_ptr<node> {
			if (first == last) {
				return nullptr;
			}

			auto head = std::make_unique<node>(node{*first, nullptr, nullptr});
			auto current = head.get();
			for (; ++first != last; current = current->next.get()) {
				current->next = std::make_unique<node>(node{*first, nullptr, current});
			}

			current->next = std::make_unique<node>(node{std::nullopt, nullptr, current});
			return head;
		}
	};

	template<typename T>
	class list<T>::iterator {
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		iterator() = default;

		auto operator*() const noexcept -> value_type const& {
			return *pointee_->value;
		}

		auto operator++() -> iterator& {
			pointee_ = pointee_->next.get();
			return *this;
		}

		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}

		auto operator--() -> iterator& {
			pointee_ = pointee_->prev;
			return *this;
		}

		auto operator--(int) -> iterator {
			auto temp = *this;
			--*this;
			return temp;
		}

		auto operator==(iterator const&) const -> bool = default;

	private:
		node* pointee_;

		friend class list<T>;

		explicit iterator(node* pointee)
		: pointee_(pointee) {}
	};
} // namespace cs6771

auto main() -> int {
	auto const l = cs6771::list<int>{0, 1, 2, 3, 4, 5};

	for (auto const i : l) {
		std::cout << i << '\n';
	}

	for (auto const i : ranges::views::reverse(l)) {
		std::cout << i << '\n';
	}
}
