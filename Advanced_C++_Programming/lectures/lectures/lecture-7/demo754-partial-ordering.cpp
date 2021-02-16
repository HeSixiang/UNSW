#include <iostream>
#include <type_traits>

template<typename T>
concept integral = std::is_integral_v<T>;

template<typename T>
concept floating_point = std::is_floating_point_v<T>;

template<typename T>
concept signed_integral = integral<T>and std::is_signed_v<T>;

auto hello(integral auto) -> void {
	std::cout << "Integral case\n";
}

auto hello(floating_point auto) -> void {
	std::cout << "Floating-point case\n";
}

auto hello(auto) -> void {
	std::cout << "Base-case\n";
}

auto hello(signed_integral auto) -> void {
	std::cout << "Signed integral case\n";
}

auto main() -> int {
	hello(0); // prints "Signed integral case"
	hello(0.0); // prints "Floating-point case"
	hello("base case"); // prints "Base-case"
	hello(0U); // prints "Integral case"
}
