#ifndef RECORD_HPP
#define RECORD_HPP

#include <map>
#include <ostream>
#include <sstream>
#include <utility>

namespace q3 {
	class record {
	private:
		std::map<std::string, std::string> data_set_;

	public:
		record()
		: data_set_({}) {}

		// get_value takes a string, returns a string
		auto get_value(std::string key) const -> const std::string& {
			return this->data_set_.find(key)->second;
		}

		// set_value takes two strings, return void
		auto set_value(std::string key, std::string value) -> void {
			auto it = this->data_set_.find(key);
			if (it != this->data_set_.end()) {
				// update
				it->second = value;
			}
			else {
				this->data_set_.insert(std::make_pair(key, value));
			}
		}

		// has_attribute takes a string, return bool
		auto has_attribute(std::string key) const -> bool {
			return !(this->data_set_.find(key) == this->data_set_.end());
		}

		// count takes no parameters, returns size_t
		auto count() const -> std::size_t {
			return this->data_set_.size();
		}

		// delete_attribute takes a string, return bool
		auto delete_attribute(std::string key) -> bool {
			return this->data_set_.erase(key) == 1;
		}

		// operator<< - write out to an ostream
		friend auto operator<<(std::ostream& os, record const& r) -> std::ostream& {
			os << "{\n";
			for (auto const& x : r.data_set_) {
				os << x.first << '=' << x.second << std::endl;
			}
			os << "}\n";
			return os;
		}

		// operator>> - read in from an istream
		friend auto operator>>(std::istream& is, record& r) -> std::istream& {
			auto s = std::string{};
			is >> s;
			while (is >> s) {
				if (s == "}") {
					break;
				}
				auto index = s.find("=");
				r.set_value(s.substr(0, index - 1), s.substr(index + 1, s.size() - 1));
			}
			return is;
		}
	};
} // namespace q3

#endif // RECORD_HPP
