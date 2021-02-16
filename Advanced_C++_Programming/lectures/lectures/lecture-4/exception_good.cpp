#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

auto main() -> int {
	try {
		throw std::out_of_range{"this is my error message!"};
	} catch (std::exception const& e) {
		std::cout << "caught std::exception\n";
	} catch (std::out_of_range const& e) {
		std::cout << "Caught out of range: " << e.what() << "\n";
	} catch (...) {
		std::cout << "caught anything\n";
	}
}