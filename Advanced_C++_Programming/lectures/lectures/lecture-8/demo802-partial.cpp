#include "./demo802-partial.h"

auto main() -> int {
	int i1 = 6771;
	int i2 = 1917;

	stack<int> s1;
	s1.push(i1);
	s1.push(i2);
	std::cout << s1.size() << " ";
	std::cout << s1.top() << " ";
	std::cout << s1.sum() << "\n";

	stack<int*> s2;
	s2.push(&i1);
	s2.push(&i2);
	std::cout << s2.size() << " ";
	std::cout << *(s2.top()) << " ";
	std::cout << s2.sum() << "\n";
}
