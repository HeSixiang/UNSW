#include <iostream>

#include "./demo801-default.h"

auto main() -> int {
	stack<float> fs;
	stack<int> is1, is2, is3;
	std::cout << stack<float>::num_stacks_ << "\n";
	std::cout << stack<int>::num_stacks_ << "\n";
}