#include <iostream>

template<typename T>
concept type = true;

template<type T> // same as using `typename` right now
auto hello(T) -> void {
	std::cout << "Permissive, base-case\n";
}

auto main() -> int {
	hello(0);
	hello(0.0);
	hello("base-case");
}
