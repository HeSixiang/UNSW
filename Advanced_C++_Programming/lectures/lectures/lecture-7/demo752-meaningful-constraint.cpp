#include <iostream>
#include <type_traits>

template<typename T>
concept integral = std::is_integral_v<T>;

template<integral T>
auto hello(T) -> void {
	std::cout << "Integral case\n";
}

auto main() -> int {
	hello(0);
	// hello(0.0);
	// hello("base-case");
}
