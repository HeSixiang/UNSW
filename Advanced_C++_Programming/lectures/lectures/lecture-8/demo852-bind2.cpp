#include <iostream>

template<typename T>
auto print(T&& a) -> void {
	std::cout << a << "\n";
}

auto goo() -> std::string const {
	return 5;
}

auto main() -> int {
	int j = 1;
	int const& k = 1;

	print(1); // rvalue,       foo(int&&)
	print(goo()); // rvalue        foo(const int&&)
	print(j); // lvalue        foo(int&)
	print(k); // const lvalue  foo(const int&)
}