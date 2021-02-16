#include <iostream>
#include <stdexcept>
#include <vector>

#include <fmt/format.h>

/*int std::vector<int>::at(std::vector<int>::size_type index) {
   if (!(index < this->size())) {
      throw std::out_of_range{"Index is not valid"};
   }
}*/

auto main() -> int {
	std::cout << "Enter -1 to quit\n";
	auto item = std::vector<int>{0, 0, 0, 0};
	std::cout << "Enter an index: ";
	for (int print_index; std::cin >> print_index;) {
		if (print_index == -1) {
			break;
		}
		try {
			std::cout << item.at(static_cast<std::vector<int>::size_type>(print_index)) << '\n';
		} catch (std::out_of_range e) {
			std::cout << fmt::format("Invalid index at {}", print_index) << "\n";
		} catch (std::invalid_argument e) {
			std::cout << "Invalid argument\n";
		} catch (...) {
			std::cout << "Something went wrong\n";
		}
		std::cout << "Enter an index: ";
	}
}