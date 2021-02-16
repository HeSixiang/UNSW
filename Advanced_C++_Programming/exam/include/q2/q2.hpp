#ifndef Q2_HPP
#define Q2_HPP

#include <concepts/concepts.hpp>
#include <exception>
#include <istream>
#include <iterator>
#include <map>
#include <memory>
#include <range/v3/utility.hpp>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace q2 {

	class matrix_error : public std::exception {
	public:
		explicit matrix_error(std::string const& what) noexcept
		: msg_(what) {}

		explicit matrix_error(const char* what) noexcept
		: msg_(what) {}

		virtual const char* what() const throw() {
			return msg_.c_str();
		}

		virtual ~matrix_error() throw() {}

	protected:
		std::string msg_;
	};

	template<typename I = int>
	class sparse_matrix {
	public:
		// static std::size_t matrix_count;
		class iterator {
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = ranges::common_tuple<std::size_t, std::size_t, I>;
			using difference_type = std::ptrdiff_t;

			iterator() = default;

			// Member functions go here

		private:
			iterator(int /* to be modified by you (int is a stub) */);
		};

		using reverse_iterator = std::reverse_iterator<iterator>;

		static auto identity(std::size_t sz) -> sparse_matrix;

		/*
		 * Must be amortised according to min((m × n) ÷ 5, 1000)
		 */
		sparse_matrix(std::size_t dim = 1) {
			auto vals_size = std::min(1000, static_cast<int>((dim * dim) / 5));
			sparse_matrix(static_cast<size_t>(vals_size), static_cast<size_t>(vals_size));
		}
		sparse_matrix(std::size_t rows, std::size_t columns)
		: ridx_({})
		, row_(rows)
		, col_(columns)
		, size_(0) {
			this->max_size_ =
			   static_cast<size_t>(std::min(1000, static_cast<int>((rows * columns) / 5)));

			this->vals_ = std::make_unique<I[]>(static_cast<size_t>(this->max_size_));
			this->cidx_ = std::make_unique<std::size_t[]>(static_cast<size_t>(this->max_size_));
			// ++matrix_count;
		}

		sparse_matrix(std::vector<std::vector<int>> data) {
			this->vals_ = std::make_unique<I[]>(1);
			this->cidx_ = std::make_unique<std::size_t[]>(1);
			this->ridx_ = {};
			this->max_size_ = 1;
			this->size_ = 0;
			this->row_ = data.size();
			this->col_ = data.size();
			for (size_t i = 0; i < data.size(); ++i) {
				for (size_t j = 0; j < data[i].size(); ++j) {
					if (data[i][j] != 0) {
						this->element(i, j, data[i][j]);
					}
				}
			}
		}

		sparse_matrix(std::istream& is) {
			auto s = std::string{};
			is >> s; // first one
			// (A, B, C)
			auto first_index = s.find('(');
			auto second_index = s.find(',');
			auto third_index = s.find(',', second_index + 1);
			auto fourth_index = s.find(')');
			auto first = std::stoi(s.substr(first_index + 1, second_index - first_index - 1));
			auto second = std::stoi(s.substr(second_index + 1, third_index - second_index - 1));
			auto third = std::stoi(s.substr(third_index + 1, third_index - fourth_index - 1));
			this->row_ = first;
			this->col_ = second;
			this->max_size_ = third;
			this->size_ = third;
			this->vals_ = std::make_unique<I[]>(third);
			this->cidx_ = std::make_unique<std::size_t>(third);
			this->ridx_ = {};
			auto pre_count = 0;
			auto current_count = 0;
			auto current_level = 0;
			auto current_index = 0;
			while (is >> s) {
				first_index = s.find('(');
				second_index = s.find(',');
				third_index = s.find(',', second_index + 1);
				fourth_index = s.find(')');
				first = std::stoi(s.substr(first_index + 1, second_index - first_index - 1));
				second = std::stoi(s.substr(second_index + 1, third_index - second_index - 1));
				third = std::stoi(s.substr(third_index + 1, third_index - fourth_index - 1));
				++current_count;
				if (current_level != first) {
					if (current_count != pre_count) {
						this->ridx_.insert(
						   std::make_pair(current_level, std::make_pair(pre_count, current_count)));
					}
					pre_count = current_count;
					current_level = first;
				}
				this->ridx_[current_index] = second;
				this->vals_[current_index] = third;
				++current_index;
			}
			// ++matrix_count;
		}

		sparse_matrix(sparse_matrix const& other) {
			this->size_ = other.size_;
			this->max_size_ = this->size_;
			this->vals_ = std::make_unique<I[]>(this->size_);
			for (size_t i = 0; i < this->size_; i++) {
				this->vals_[i] = other.vals_[i];
			}
			this->cidx_ = std::make_unique<std::size_t>(this->size_);
			for (size_t i = 0; i < this->size_; i++) {
				this->cidx_[i] = other.cidx_[i];
			}
			this->ridx_ = std::map<std::size_t, std::pair<std::size_t, std::size_t>>{other.ridx_};
			this->row_ = other.row;
			this->col_ = other.col;
			// ++matrix_count;
		}

		sparse_matrix(sparse_matrix&& other) noexcept = default;

		~sparse_matrix() noexcept = default;
		// --matrix_count;

		auto operator=(sparse_matrix const& other) -> sparse_matrix& {
			auto copy = sparse_matrix(other);
			ranges::swap(this->vals_, copy.vals_);
			ranges::swap(this->cidx_, copy.cidx_);
			ranges::swap(this->ridx_, copy.ridx_);
			ranges::swap(this->row_, copy.row_);
			ranges::swap(this->col_, copy.col_);
			ranges::swap(this->size_, copy.size_);
			return *this;
		}
		auto operator=(sparse_matrix&& other) noexcept -> sparse_matrix& {
			auto copy = sparse_matrix(other);
			ranges::swap(this->vals_, copy.vals_);
			ranges::swap(this->cidx_, copy.cidx_);
			ranges::swap(this->ridx_, copy.ridx_);
			ranges::swap(this->row, copy.row);
			ranges::swap(this->col, copy.col);
			ranges::swap(this->size_, copy.size_);
			return *this;
		}

		auto operator+=(sparse_matrix const& other) -> sparse_matrix& {
			if (this->col != other.col || this->row != other.row) {
				throw matrix_error("matrices must have identical dimensions");
			}
			for (auto const& [key, val] : other.ridx_) {
			}
		}

		auto operator-=(sparse_matrix const& other) -> sparse_matrix&;
		auto operator*=(sparse_matrix const& other) -> sparse_matrix&;
		auto operator==(sparse_matrix const& other) const noexcept -> bool;

		friend auto operator+(sparse_matrix const& lhs, sparse_matrix const& rhs) -> sparse_matrix;
		friend auto operator-(sparse_matrix const& lhs, sparse_matrix const& rhs) -> sparse_matrix;
		friend auto operator*(sparse_matrix const& lhs, sparse_matrix const& rhs) -> sparse_matrix;
		friend auto operator<<(std::ostream& os, sparse_matrix const&) -> std::ostream& {
			return os;
		}
		friend auto operator>>(std::istream& is, sparse_matrix& sm) -> std::istream&;
		friend auto transpose(sparse_matrix const& sm) -> sparse_matrix;

		auto rows() const noexcept -> std::size_t {
			return this->row_;
		}
		auto cols() const noexcept -> std::size_t {
			return this->col_;
		}
		auto element(std::size_t i, std::size_t j, I val) -> void {
			if (i >= this->row_ || j >= this->col_) {
				throw matrix_error("values are not in bounds");
			}

			auto row_it = this->ridx_.find(i);
			if (row_it != this->ridx_.end()) {
				auto value_pair = row_it->second;
				auto start_index = value_pair.first;
				auto end_index = start_index + value_pair.second;
				for (size_t e = start_index; e < end_index; ++e) {
					if (this->cidx_[e] == j) {
						this->vals_[e] = val;
						return;
					}
				}
				insert(i, j, val);
			}
			else {
				insert(i, j, val);
			}
		}

		auto insert(std::size_t i, std::size_t j, I val) -> void {
			if (this->max_size_ == this->size_) {
				growSize();
			}
			auto row_it = this->ridx_.find(i);
			size_t index = 0;
			if (row_it != this->ridx_.end()) {
				row_it->second.second++;
				index = row_it->second.first + row_it->second.second - 1;
			}
			else {
				auto temp_i = i;
				if (temp_i != 0) {
					do {
						--temp_i;
					} while (temp_i != 0 && this->ridx_.find(temp_i) == this->ridx_.end());
				}
				if (temp_i != 0) {
					auto pre_it = this->ridx_.find(temp_i);
					auto pre_start = pre_it->second.first + pre_it->second.second;
					index = pre_start - 1;
					this->ridx_.insert(std::make_pair(i, std::make_pair(pre_start, 1)));
				}
				else {
					if (this->ridx_.find(0) == this->ridx_.end()) {
						this->ridx_.insert(std::make_pair(0, std::make_pair(0, 1)));
					}
					else {
						auto pre_it = this->ridx_.find(temp_i);
						auto pre_start = pre_it->second.first + pre_it->second.second;
						index = pre_start - 1;
						this->ridx_.insert(std::make_pair(i, std::make_pair(pre_start, 1)));
					}
				}
			}
			for (size_t e = i + 1; e < this->row_; ++e) {
				if (this->ridx_.find(e) != this->ridx_.end()) {
					this->ridx_.find(e)->second.first++;
				}
			}
			if (this->size_ != 0) {
				for (size_t e = this->size_ - 1; e >= index; e--) {
					// shift all element right
					// from given index
					this->vals_[e + 1] = this->vals_[e];
				}
			}

			this->vals_[index] = val;

			if (this->size_ != 0) {
				for (size_t e = this->size_ - 1; e >= index; e--) {
					// shift all element right
					// from given index
					this->cidx_[e + 1] = this->cidx_[e];
				}
			}

			// insert data at given index
			this->cidx_[index] = j;
			this->size_++;
		}

		// double size
		auto growSize() -> void {
			auto current_size = this->max_size_;
			this->max_size_ = 2 * current_size;
			auto new_array = std::make_unique<I[]>(current_size * 2);
			for (size_t i = 0; i < current_size; ++i) {
				new_array[i] = this->vals_[i];
			}
			this->vals_ = std::move(new_array);
			auto new_array_2 = std::make_unique<std::size_t[]>(current_size * 2);
			for (size_t i = 0; i < current_size; ++i) {
				new_array_2[i] = this->cidx_[i];
			}
			this->cidx_ = std::move(new_array_2);
		}

		auto element(std::size_t i, std::size_t j) const -> I const {
			if (i >= this->row_ || j >= this->col_) {
				throw matrix_error("values are not in bounds");
			}
			// const int empty = std::make_unique<I>();
			auto row_it = this->ridx_.find(i);
			if (row_it == this->ridx_.end()) {
				return 0;
			}
			auto value_pair = row_it->second;
			auto start_index = value_pair.first;
			auto end_index = start_index + value_pair.second;
			for (size_t e = start_index; e < end_index; ++e) {
				if (this->cidx_[e] == i) {
					return this->vals_[e];
				}
			}
			return 0;
		}
		auto transpose() -> sparse_matrix&;

		auto begin() const -> iterator;
		auto end() const -> iterator;

		auto rbegin() const -> reverse_iterator;
		auto rend() const -> reverse_iterator;

	private:
		std::unique_ptr<I[]> vals_;
		std::unique_ptr<std::size_t[]> cidx_;
		std::map<std::size_t, std::pair<std::size_t, std::size_t>> ridx_;
		std::size_t row_;
		std::size_t col_;
		std::size_t size_;
		std::size_t max_size_;
	};

	// template<typename I = int>
	// std::size_t sparse_matrix<int>::matrix_count = 0;

} // namespace q2

#endif // Q2_HPP
