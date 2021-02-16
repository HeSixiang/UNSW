template<typename T>
T min(T a, T b) {
	return a < b ? a : b;
}

auto main() -> int {
	int i = 0;
	double d = 0;
	min(i, static_cast<int>(d)); // int min(int, int)
	// min<int>(i, d); // int min(int, int)
	min(static_cast<double>(i), d); // double min(double, double)
	min<double>(i, d); // double min(double, double)
}