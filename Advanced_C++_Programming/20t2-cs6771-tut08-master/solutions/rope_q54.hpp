#ifndef COMP6771_ROPE_HPP
#define COMP6771_ROPE_HPP

#include <iterator>
#include <range/v3/iterator.hpp>
#include <string>
#include <utility>
#include <vector>

template<bool is_const, typename T>
struct maybe_const {
	using type = T;
};

template<typename T>
struct maybe_const<true, T> {
	using type = T const;
};

template<bool is_const, typename T>
using maybe_const_t = typename maybe_const<is_const, T>::type;

class rope {
	template<bool is_const>
	class iterator_impl {
		using outer_iterator = std::conditional_t<is_const,
		                                          std::vector<std::string>::const_iterator,
		                                          std::vector<std::string>::iterator>;
		using inner_iterator = std::conditional_t<is_const,
		                                          typename outer_iterator::value_type::const_iterator,
		                                          typename outer_iterator::value_type::iterator>;

	public:
		using value_type = typename inner_iterator::value_type;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		friend class iterator_impl<not is_const>;

		iterator_impl() = default;

		explicit iterator_impl(maybe_const_t<is_const, std::vector<std::string>>& pointee,
		                       outer_iterator outer,
		                       inner_iterator inner) noexcept
		: pointee_(&pointee)
		, outer_(outer)
		, inner_(inner) {}

		// A const_iterator should be able to construct a non-const_iterator, but not the other way
		// around, so we disable it for the non-const case.
		// NOLINTNEXTLINE(google-explicit-constructor)
		explicit(false) iterator_impl(iterator_impl<not is_const> const& other) requires is_const
		: pointee_(other.pointee_)
		, outer_(other.outer_)
		, inner_(other.inner_) {}

		// Indirect mutability isn't a property of const_iterators
		auto operator*() const -> value_type& requires(not is_const) {
			return *inner_;
		}

		// This will never be called for a non-const_iterator, so we can exclude it from being
		// generated.
		auto operator*() const -> value_type requires is_const {
			return *inner_;
		}

		auto operator++() -> iterator_impl& {
			if (inner_ != outer_->end()) {
				++inner_;

				if (inner_ != outer_->end()) {
					return *this;
				}
			}

			++outer_;
			inner_ = outer_ == pointee_->end() ? inner_iterator() : outer_->begin();
			return *this;
		}

		auto operator++(int) -> iterator_impl {
			auto temp = *this;
			++*this;
			return temp;
		}

		auto operator--() -> iterator_impl& {
			if (inner_ == inner_iterator()) {
				outer_ = ranges::prev(pointee_->end());
				inner_ = ranges::prev(outer_->end());
				return *this;
			}

			if (inner_ != outer_->begin()) {
				--inner_;
				return *this;
			}

			--outer_;
			inner_ = ranges::prev(outer_->end());
			return *this;
		}

		auto operator--(int) -> iterator_impl {
			auto temp = *this;
			--*this;
			return temp;
		}

		friend auto operator==(iterator_impl, iterator_impl) -> bool = default;

	private:
		maybe_const_t<is_const, std::vector<std::string>>* pointee_ = nullptr;
		outer_iterator outer_;
		inner_iterator inner_;
	};

public:
	using iterator = iterator_impl<false>;
	using const_iterator = iterator_impl<true>;

	rope() = default;

	explicit rope(std::vector<std::string> rope)
	: rope_{std::move(rope)} {}

	[[nodiscard]] auto begin() -> iterator {
		return begin_impl(*this);
	}

	[[nodiscard]] auto end() -> iterator {
		return end_impl(*this);
	}

	[[nodiscard]] auto begin() const -> const_iterator {
		return begin_impl(*this);
	}

	[[nodiscard]] auto end() const -> const_iterator {
		return end_impl(*this);
	}

private:
	std::vector<std::string> rope_;

	// Detects whether or not self is const-qualified (deduced via T) and constructs the appropriate
	// iterator_impl.
	template<typename T>
	[[nodiscard]] static auto begin_impl(T& self) -> decltype(self.begin()) {
		auto inner = self.rope_.empty() ? decltype(self.rope_.begin()->begin()){}
		                                : self.rope_.begin()->begin();
		return iterator_impl<std::is_const_v<T>>(self.rope_, self.rope_.begin(), std::move(inner));
	}

	template<typename T>
	[[nodiscard]] static auto end_impl(T& self) -> decltype(self.end()) {
		return iterator_impl<std::is_const_v<T>>(self.rope_, self.rope_.end(), {});
	}
};

#endif // COMP6771_ROPE_HPP
