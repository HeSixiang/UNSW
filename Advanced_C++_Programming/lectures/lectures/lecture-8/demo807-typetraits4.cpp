#include <type_traits>
#include <iostream>

template<typename T>
auto testIfNumberType(T i) -> void {
	if (std::is_integral<T>::value || std::is_floating_point<T>::value) {
		std::cout << i << " is a number"
		          << "\n";
	}
	else {
		std::cout << i << " is not a number"
		          << "\n";
	}
}

auto main() -> int {
	int i = 6;
	long l = 7;
	double d = 3.14;
	testIfNumberType(i);
	testIfNumberType(l);
	testIfNumberType(d);
	testIfNumberType(123);
	testIfNumberType("Hello");
	auto s = "World";
	testIfNumberType(s);
}