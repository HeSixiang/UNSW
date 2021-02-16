#ifndef QUERY_HPP
#define QUERY_HPP

#include "q3/record.hpp"
#include <string>
namespace q3 {
	class record;

	class query {
	public:
		virtual ~query() = default;
		[[nodiscard]] auto virtual matches(record const&) const -> bool = 0;
	};

	class query_equals : public query {
	private:
		std::string column_;
		std::string value_;

	public:
		// query_equals(column, value);
		query_equals(std::string column, std::string value)
		: column_(column)
		, value_(value){};

		auto matches(record const& r) const -> bool override {
			if (r.has_attribute(this->column_)) {
				return r.get_value(this->column_) == this->value_;
			}
			return false;
		}
	};

	class query_less_than : public query {
	private:
		std::string column_;
		std::string value_;

	public:
		query_less_than(std::string column, std::string value)
		: column_(column)
		, value_(value){};

		auto matches(record const& r) const -> bool override {
			if (r.has_attribute(this->column_)) {
				return r.get_value(this->column_) < this->value_;
			}
			return false;
		}
	};

	class query_greater_than : public query {
	private:
		std::string column_;
		std::string value_;

	public:
		query_greater_than(std::string column, std::string value)
		: column_(column)
		, value_(value){};

		auto matches(record const& r) const -> bool override {
			if (r.has_attribute(this->column_)) {
				return r.get_value(this->column_) > this->value_;
			}
			return false;
		}
	};

	class query_starts_with : public query {
	private:
		std::string column_;
		std::string value_;

	public:
		query_starts_with(std::string column, std::string value)
		: column_(column)
		, value_(value){};

		auto matches(record const& r) const -> bool override {
			if (r.has_attribute(this->column_)) {
				return r.get_value(this->column_).starts_with(this->value_);
			}
			return false;
		}
	};

	class query_and : public query {
	private:
		const query& one_;
		const query& two_;

	public:
		query_and(const query& one, const query& two)
		: one_(one)
		, two_(two){};
		auto matches(record const& r) const -> bool override {
			return this->one_.matches(r) && this->two_.matches(r);
		}
	};

	class query_or : public query {
	private:
		const query& one_;
		const query& two_;

	public:
		query_or(const query& one, const query& two)
		: one_(one)
		, two_(two){};
		auto matches(record const& r) const -> bool override {
			return this->one_.matches(r) || this->two_.matches(r);
		}
	};

	class query_not : public query {
	private:
		const query& one_;

	public:
		query_not(const query& one)
		: one_(one){};
		auto matches(record const& r) const -> bool override {
			return !this->one_.matches(r);
		}
	};

} // namespace q3

#endif // QUERY_HPP
