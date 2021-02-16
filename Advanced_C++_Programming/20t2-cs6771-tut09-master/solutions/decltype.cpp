auto main() -> int {
	int i = 5;
	int& k = i;

	decltype(i) x; // int; - variable
	decltype(k) y; // int& - lvalue
	decltype(std::move(i)) z; // int&& - rvalue
	decltype(4.2);   // double - rvalue	
}