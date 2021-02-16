#include <iostream>
#include <type_traits>

template<typename T>
concept floating_point = std::is_floating_point_v<T>;

auto hello(floating_point auto) -> void {
	std::cout << "Floating-point case\n";
}

auto main() -> int {
	// hello(0);
	hello(0.0);
	// hello("base case");
}
