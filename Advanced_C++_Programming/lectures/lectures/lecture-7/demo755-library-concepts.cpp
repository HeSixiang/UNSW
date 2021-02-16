#include <concepts/concepts.hpp>
#include <iostream>
#include <type_traits>

auto hello(concepts::integral auto) -> void {
	std::cout << "Integral case\n";
}

auto hello(auto) -> void {
	std::cout << "Base-case\n";
}

auto hello(concepts::signed_integral auto) -> void {
	std::cout << "Signed integral case\n";
}

int main() {
	hello(0); // prints "Signed integral case"
	hello("base case"); // prints "Base-case"
	hello(0U); // prints "Integral case"
}
