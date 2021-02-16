#include <iostream>
#include <vector>

#include "q2/q2.hpp"

auto main() -> int {
	// auto m = q2::sparse_matrix{};
	// std::cout << m;

	auto data = std::vector<std::vector<int>>{};
	auto temp = std::vector<int>{1, 0, 0, 2, 0, 0, 3};
	data.push_back(temp);
	temp = std::vector<int>{4, 5, 0, 0, 0, 0, 0};
	data.push_back(temp);
	temp = std::vector<int>{0, 6, 7, 0, 0, 8, 0};
	data.push_back(temp);
	temp = std::vector<int>{9, 0, 0, 10, 11, 12, 0};
	data.push_back(temp);
	temp = std::vector<int>{0, 13, 0, 0, 14, 0, 0};
	data.push_back(temp);
	temp = std::vector<int>{0, 0, 0, 0, 0, 0, 0};
	data.push_back(temp);
	temp = std::vector<int>{0, 0, 0, 0, 0, 0, 0};
	data.push_back(temp);
	temp = std::vector<int>{0, 0, 0, 0, 0, 0, 15};
	data.push_back(temp);
	auto m = q2::sparse_matrix(data);
	std::cout << m.element(2, 2) << std::endl;
}
