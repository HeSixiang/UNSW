#include <iostream>

template<typename T>
concept type = true;

template<typename T>
requires type<T> //
auto hello(T) -> void {
	std::cout << "Permissive, base-case\n";
}

auto main() -> int {
	hello(0);
	hello(0.0);
	hello("base-case");
}
