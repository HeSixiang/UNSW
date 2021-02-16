// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fmt/ostream.h>
#include <functional>
#include <span>

namespace comp6771 {
	// A constructor that generates a euclidean vector with a dimension of 1 and magnitude of 0.0.
	euclidean_vector::euclidean_vector() noexcept
	: euclidean_vector(1, 0.0) {}

	// A constructor that takes the number of dimensions (as a int) but no magnitudes,
	// sets the magnitude in each dimension as 0.0.
	euclidean_vector::euclidean_vector(int dem) noexcept
	: euclidean_vector(dem, 0.0) {}

	// A constructor that takes the number of dimensions (as an int) and initialises
	// magnitude in each dimension as the second argument (a double).
	euclidean_vector::euclidean_vector(int dem, double mag) noexcept
	: dimension_(dem)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	, magnitudes_(std::make_unique<double[]>(static_cast<size_t>(dem))) {
		ranges::fill(std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_)), mag);
	}

	// A constructor (or constructors) that takes the start and end of an iterator vector
	// and works out the required dimensions,
	// set magnitude in each dimension according to the iterated values.
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end) noexcept
	: euclidean_vector(static_cast<int>(end - begin)) {
		ranges::copy(begin,
		             end,
		             std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_)).begin());
	}

	// A constructor that takes an initialiser list of doubles to populate vector magnitudes.
	euclidean_vector::euclidean_vector(std::initializer_list<double> list) noexcept
	: euclidean_vector(static_cast<int>(list.size())) {
		ranges::copy(list,
		             std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_)).begin());
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& another) noexcept
	: euclidean_vector(another.dimension_) {
		ranges::copy(
		   std::span<double>(another.magnitudes_.get(), static_cast<size_t>(another.dimension_)),
		   std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_)).begin());
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& another) noexcept
	: euclidean_vector(another.dimension_) {
		another.dimension_ = 0; // Leave a moved-from object in a state with 0 dimensions
		this->magnitudes_ = ranges::move(another.magnitudes_);
	}

	// A copy assignment operator overload
	auto euclidean_vector::operator=(euclidean_vector const& orig) noexcept -> euclidean_vector& {
		auto temp = euclidean_vector(orig);
		ranges::swap(this->dimension_, temp.dimension_);
		ranges::swap(this->magnitudes_, temp.magnitudes_);
		return *this;
	}

	// A move assignment operator
	auto euclidean_vector::operator=(euclidean_vector&& orig) noexcept -> euclidean_vector& {
		auto temp = euclidean_vector(orig);
		ranges::swap(this->dimension_, temp.dimension_);
		ranges::swap(this->magnitudes_, temp.magnitudes_);
		return *this;
	}

	// create reference in a given dimension of the Euclidean vector
	auto euclidean_vector::operator[](int index) noexcept -> double& {
		assert(index >= 0 && index < this->dimension_);
		this->norm_ = -1.0; // -1.0 means invalid, this function might change inside data
		return this->magnitudes_[static_cast<size_t>(index)];
	}

	// copy value in a given dimension of the Euclidean vector
	auto euclidean_vector::operator[](int index) const noexcept -> double {
		assert(index >= 0 && index < this->dimension_);
		return this->magnitudes_[static_cast<size_t>(index)];
	}

	// Returns a copy of the current object
	auto euclidean_vector::operator+() const noexcept -> euclidean_vector {
		return euclidean_vector(*this);
	}

	// Returns a copy of the current object, where each scalar value has its sign negated
	auto euclidean_vector::operator-() const noexcept -> euclidean_vector {
		auto result = euclidean_vector(*this);
		// auto result = euclidean_vector(this->dimension_);
		auto result_span =
		   std::span<double>(result.magnitudes_.get(), static_cast<size_t>(result.dimension_));
		std::transform(result_span.begin(), result_span.end(), result_span.begin(), [](double x) {
			return -x;
		});
		return result;
	}

	// For adding vectors of the same dimension
	auto euclidean_vector::operator+=(const euclidean_vector& another) -> euclidean_vector& {
		if (another.dimension_ != this->dimension_) {
			throw euclidean_vector_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                         this->dimension_,
			                                         another.dimension_));
		}
		auto current_span =
		   std::span<double>(this->magnitudes_.get(), static_cast<size_t>(this->dimension_));
		auto another_span =
		   std::span<double>(another.magnitudes_.get(), static_cast<size_t>(another.dimension_));
		std::transform(current_span.begin(),
		               current_span.end(),
		               another_span.begin(),
		               current_span.begin(),
		               std::plus<>());
		return *this;
	}

	// For subtracting vectors of the same dimension
	auto euclidean_vector::operator-=(const euclidean_vector& another) -> euclidean_vector& {
		if (another.dimension_ != this->dimension_) {
			throw euclidean_vector_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                         this->dimension_,
			                                         another.dimension_));
		}
		auto current_span =
		   std::span<double>(this->magnitudes_.get(), static_cast<size_t>(this->dimension_));
		auto another_span =
		   std::span<double>(another.magnitudes_.get(), static_cast<size_t>(another.dimension_));
		std::transform(current_span.begin(),
		               current_span.end(),
		               another_span.begin(),
		               current_span.begin(),
		               std::minus<>());
		return *this;
	}

	// For scalar multiplication,
	auto euclidean_vector::operator*=(double factor) noexcept -> euclidean_vector& {
		auto span = std::span<double>(this->magnitudes_.get(), static_cast<size_t>(this->dimension_));
		std::transform(span.begin(), span.end(), span.begin(), [factor](double x) {
			return x *= factor;
		});
		return *this;
	}

	// For scalar division
	auto euclidean_vector::operator/=(double factor) -> euclidean_vector& {
		if (factor == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}
		auto span = std::span<double>(this->magnitudes_.get(), static_cast<size_t>(this->dimension_));
		std::transform(span.begin(), span.end(), span.begin(), [factor](double x) {
			return x /= factor;
		});
		return *this;
	}

	// Operators for type casting to vector
	euclidean_vector::operator std::vector<double>() const noexcept {
		auto temp = std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_));
		return std::vector<double>(temp.begin(), temp.end());
	}

	// Operators for type casting to list
	euclidean_vector::operator std::list<double>() const noexcept {
		auto temp = std::span<double>(magnitudes_.get(), static_cast<size_t>(dimension_));
		return std::list<double>(temp.begin(), temp.end());
	}

	// Return the number of dimensions in a particular euclidean_vector
	auto euclidean_vector::dimensions() const noexcept -> int {
		return this->dimension_;
	}

	// Returns the value of the magnitude in the dimension given as the function parameter
	auto euclidean_vector::at(int index) const -> double {
		if (index < 0 || index >= this->dimension_) {
			throw euclidean_vector_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", index));
		}
		return this->magnitudes_[static_cast<size_t>(index)];
	}

	// Returns the reference of the magnitude in the dimension given as the function parameter
	auto euclidean_vector::at(int index) -> double& {
		if (index < 0 || index >= this->dimension_) {
			throw euclidean_vector_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", index));
		}
		this->norm_ = -1.0; // -1.0 means invalid, this function might change inside data
		return this->magnitudes_[static_cast<size_t>(index)];
	}

	// this function is static, help to update the norm (cache)
	auto euclidean_vector::euclidean_norm_helper(euclidean_vector const& vector) -> double {
		if (vector.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a "
			                             "norm");
		}
		if (vector.norm_ == -1.0) {
			// first time, need to caculate norm
			auto sum = 0.0;
			for (auto i = 0; i < vector.dimensions(); ++i) {
				sum += pow(vector.at(i), 2);
			}
			vector.norm_ = sqrt(sum);
		}
		return vector.norm_;
	}

	// Returns the Euclidean norm of the vector as a double
	auto euclidean_norm(euclidean_vector const& vector) -> double {
		try {
			return euclidean_vector::euclidean_norm_helper(vector);
		} catch (euclidean_vector_error const& e) {
			throw e;
		}
	}

	// Returns a Euclidean vector that is the unit vector of input vector
	auto unit(euclidean_vector const& vector) -> euclidean_vector {
		if (vector.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}
		auto norm = euclidean_norm(vector);
		if (norm == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a "
			                             "unit vector");
		}
		return euclidean_vector(vector) / norm;
	}

	// Computes and return the dot product of two vector
	auto dot(euclidean_vector const& one, euclidean_vector const& other) -> double {
		if (one.dimensions() != other.dimensions()) {
			throw euclidean_vector_error(fmt::format("Dimensions of LHS({}) and RHS({}) do not match",
			                                         one.dimensions(),
			                                         other.dimensions()));
		}
		auto result{0.0};
		for (auto i = 0; i < one.dimensions(); ++i) {
			result += one.at(i) * other.at(i);
		}
		return result;
	}

} // namespace comp6771
