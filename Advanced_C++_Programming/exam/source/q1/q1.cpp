#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <math.h>
#include <ostream>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <variant>

#include "q1/q1.hpp"

auto show(std::variant<int, double>& value, std::ostream& os, std::string end) -> void {
	if (value.index() == 0) {
		// os << "calling int";
		os << std::defaultfloat << std::get<int>(value) << end;
	}
	else {
		// double values should be formatted to 3 decimal places
		//	os << "calling double";
		os << std::fixed << std::setprecision(3) << std::get<double>(value) << end;
	}
}

namespace q1 {
	auto run_calculator(std::string const& filename, std::ostream& os) -> void {
		// std::cout.setf(std::ios::fixed, std::ios::floatfield);
		// std::cout.precision(3);

		// open the file for reading
		auto in = std::ifstream{};
		in.open(filename);
		if (!in.is_open()) {
			os << "open file error" << std::endl;
			exit(-1);
		}

		// string to be read into
		auto s = std::string{};

		// read the file while we have input.
		std::list<std::string> command{};
		while (in >> s) {
			command.push_back(s);
		}

		std::set<std::string> getTwo{"add", "sub", "mult", "div"};
		std::set<std::string> getOne{"sqrt", "reverse", "repeat"};

		std::stack<std::variant<int, double>> values{};
		std::vector<std::string> repeatHolder{};
		std::queue<std::variant<int, double>> reverseHolder{};
		auto isRepeat = false;
		auto repeatTime = 0;

		while (!command.empty()) {
			// std::cout << s << std::endl;
			s = command.front();
			command.pop_front();
			if (std::regex_match(s, std::regex("[1-9][0-9]*"))) {
				// int;
				values.push(std::stoi(s));
			}
			else if (std::regex_match(s, std::regex("[1-9][0-9]*[.][0-9]+"))) {
				// double
				values.push(std::stod(s));
			}
			else if (getTwo.find(s) != getTwo.end()) {
				std::variant<int, double> firstValue = values.top();
				values.pop();
				std::variant<int, double> secondValue = values.top();
				values.pop();
				std::variant<int, double> result;
				if (s == "add") {
					result = std::visit([](auto x, auto y) -> std::variant<int, double> { return x + y; },
					                    firstValue,
					                    secondValue);
					show(firstValue, os, " + ");
				}
				else if (s == "sub") {
					result = std::visit([](auto x, auto y) -> std::variant<int, double> { return x - y; },
					                    firstValue,
					                    secondValue);
					show(firstValue, os, " - ");
				}
				else if (s == "mult") {
					result = std::visit([](auto x, auto y) -> std::variant<int, double> { return x * y; },
					                    firstValue,
					                    secondValue);
					show(firstValue, os, " * ");
				}
				else {
					// s == "div"
					result = std::visit([](auto x, auto y) -> std::variant<int, double> { return x / y; },
					                    firstValue,
					                    secondValue);
					show(firstValue, os, " / ");
				}
				show(secondValue, os, " = ");
				show(result, os, "\n");
				values.push(result);
			}
			else if (getOne.find(s) != getOne.end()) {
				std::variant<int, double> firstValue = values.top();
				values.pop();
				if (s == "sqrt") {
					std::variant<int, double> result =
					   std::visit([](auto x) -> std::variant<int, double> { return sqrt(x); }, firstValue);

					os << "sqrt ";
					show(firstValue, os, " = ");
					show(result, os, "\n");
					values.push(result);
				}
				else if (s == "reverse") {
					auto reverseTime = std::get<int>(firstValue); // assume firstValue is int
					for (int i = 0; i < reverseTime; ++i) {
						reverseHolder.push(values.top());
						values.pop();
					}
					for (int i = 0; i < reverseTime; ++i) {
						values.push(reverseHolder.front());
						reverseHolder.pop();
					}
				}
				else {
					// s == "repeat"
					repeatTime = std::get<int>(firstValue) - 1; // assume firstValue is int
					isRepeat = true;
					continue;
				}
			}
			else if (s == "endrepeat") {
				isRepeat = false;
				for (int i = 0; i < repeatTime; ++i) {
					for (auto j = repeatHolder.crbegin(); j != repeatHolder.crend(); ++j) {
						command.push_front(*j);
					}
				}
				repeatHolder.clear();
				repeatTime = 0;
			}
			else if (s == "pop") {
				values.pop();
			}

			if (isRepeat) {
				repeatHolder.push_back(s);
			}
		}
		in.close();
	}
} // namespace q1
