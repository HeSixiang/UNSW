#include <iostream>
#include <istream>
#include <ostream>

#include "./demo353-io.h"

std::ostream& operator<<(std::ostream& os, point const& p) {
	os << "(" << p.x_ << "," << p.y_ << ")";
	return os;
}

std::istream& operator>>(std::istream& is, point& p) {
	// To be done in tutorials
}

auto main() -> int {
	point p(1, 2);
	std::cout << p << '\n';
}