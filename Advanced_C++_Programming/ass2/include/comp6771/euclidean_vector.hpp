#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <array>
#include <compare>
#include <cstddef>
#include <cstring>
#include <functional>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		euclidean_vector() noexcept;
		explicit euclidean_vector(int) noexcept;
		euclidean_vector(int, double) noexcept;
		euclidean_vector(std::vector<double>::const_iterator,
		                 std::vector<double>::const_iterator) noexcept;
		euclidean_vector(std::initializer_list<double>) noexcept;
		euclidean_vector(euclidean_vector const&) noexcept;
		euclidean_vector(euclidean_vector&&) noexcept;
		~euclidean_vector() noexcept = default;

		auto operator=(euclidean_vector const&) noexcept -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;
		auto operator[](int) noexcept -> double&; // setter ev[0] = double return pointer
		auto operator[](int) const noexcept -> double; // getter double = ev[0] return value
		auto operator+() const noexcept -> euclidean_vector;
		auto operator-() const noexcept -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double) noexcept -> euclidean_vector&;
		auto operator/=(double) -> euclidean_vector&;
		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() const noexcept;

		[[nodiscard]] auto dimensions() const noexcept -> int;
		[[nodiscard]] auto at(int) const -> double;
		[[nodiscard]] auto at(int) -> double&;

		// True if the two vectors are equal in the number of dimensions
		// and the magnitude in each dimension is equal.
		friend auto operator==(euclidean_vector const& one, euclidean_vector const& other) noexcept
		   -> bool {
			return (one.dimension_ == other.dimension_)
			       && ranges::equal(
			          std::span<double>(one.magnitudes_.get(), static_cast<size_t>(one.dimension_)),
			          std::span<double>(other.magnitudes_.get(), static_cast<size_t>(other.dimension_)));
		}

		// True if the two vectors are not equal in the number of dimensions
		// or the magnitude in each dimension is not equal.
		friend auto operator!=(euclidean_vector const& one, euclidean_vector const& other) noexcept
		   -> bool {
			return !(one == other);
		}

		// For adding vectors of the same dimension.
		friend auto operator+(euclidean_vector const& one, euclidean_vector const& other)
		   -> euclidean_vector {
			try {
				auto result = euclidean_vector(one);
				result += other;
				return result;
			} catch (euclidean_vector_error const& e) {
				throw e;
			}
		}

		// For substracting vectors of the same dimension.
		friend auto operator-(euclidean_vector const& one, euclidean_vector const& other)
		   -> euclidean_vector {
			try {
				auto result = euclidean_vector(one);
				result -= other;
				return result;
			} catch (euclidean_vector_error const& e) {
				throw e;
			}
		}

		// For scalar multiplication
		friend auto operator*(euclidean_vector const& current, double factor) noexcept
		   -> euclidean_vector {
			return euclidean_vector(current) *= factor;
		}

		// For scalar multiplication double value on the other side
		friend auto operator*(double factor, euclidean_vector const& current) noexcept
		   -> euclidean_vector {
			return current * factor;
		}

		// For scalar division
		friend auto operator/(euclidean_vector const& current, double factor) {
			try {
				auto result = euclidean_vector(current);
				result /= factor;
				return result;
			} catch (euclidean_vector_error const& e) {
				throw e;
			}
		}

		// Prints out the magnitude in each dimension of the Euclidean vector (surrounded by[ and ])
		friend auto operator<<(std::ostream& out, euclidean_vector const& current) noexcept
		   -> std::ostream& {
			out << '[';
			for (auto i = 0; i < current.dimension_ - 1; ++i) {
				out << current.magnitudes_[static_cast<size_t>(i)] << ' ';
			}
			if (current.dimension_ > 0) {
				out << current.magnitudes_[static_cast<size_t>(current.dimension_) - 1];
			}
			out << ']';
			return out;
		}

		// to get and change norm from private field, make it static
		static auto euclidean_norm_helper(euclidean_vector const&) -> double;

	private:
		int dimension_;
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		// for perfomance reasons, cache the norm here
		mutable double norm_ = -1.0; // -1.0 means invalid
	};

	// declare utility function
	auto euclidean_norm(euclidean_vector const& vector) -> double;
	auto unit(euclidean_vector const&) -> euclidean_vector;
	auto dot(euclidean_vector const&, euclidean_vector const&) -> double;
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
