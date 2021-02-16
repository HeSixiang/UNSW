#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "q3/query.hpp"
#include "q3/record.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
namespace q3 {
	class database {
	private:
		std::vector<record> data_;

	public:
		database()
		: data_({}){};

		// insert - takes a record, always succeeds and returns void
		auto insert(record r) -> void {
			this->data_.push_back(r);
		}

		// count returns size_t
		auto count() const -> std::size_t {
			return this->data_.size();
		}

		// operator<< - write out to an ostream
		friend auto operator<<(std::ostream& os, database const& d) -> std::ostream& {
			for (auto const& r : d.data_) {
				os << r;
			}
			return os;
		}

		// operator>> - read in from an istream
		friend auto operator>>(std::istream& is, database& d) -> std::istream& {
			auto s = std::string{};
			record r;
			while (is >> s) {
				if (s == "{") {
					r = record();
				}
				else if (s == "}") {
					d.insert(r);
				}
				else {
					auto index = s.find("=");
					r.set_value(s.substr(0, index - 1), s.substr(index + 1, s.size() - 1));
				}
			}
			return is;
		}

		// delete_matching - takes a query and deletes matching records from database
		auto delete_matching(const query& q) -> std::size_t {
			int count = 0;
			(void)std::remove_if(this->data_.begin(),
			                     this->data_.end(),
			                     [count, &q](record& r) mutable -> bool {
				                     if (q.matches(r)) {
					                     ++count;
					                     return true;
				                     }
				                     return false;
			                     });
			return static_cast<std::size_t>(count);
		}

		// select - takes a query and returns a new Database object comprised of matching rows
		auto select(const query& q) const -> database {
			database result = database();
			for (auto r : this->data_) {
				if (q.matches(r)) {
					result.insert(r);
				}
			}
			return result;
		}
	};

} // namespace q3

#endif // DATABASE_HPP
