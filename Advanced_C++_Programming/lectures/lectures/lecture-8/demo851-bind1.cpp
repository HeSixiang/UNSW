#include <iostream>

auto print(const std::string& a) -> void {
	std::cout << a << "\n";
}

auto goo() -> std::string const {
	return "C++";
}

auto main() -> int {
	std::string j = "C++";
	std::string const& k = "C++";
	print("C++"); // rvalue
	print(goo()); // rvalue
	print(j); // lvalue
	print(k); // const lvalue
}