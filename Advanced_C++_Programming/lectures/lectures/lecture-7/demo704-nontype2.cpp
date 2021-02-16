auto findmin(const std::array<int, 3> a) -> int {
	int min = a[0];
	for (int i = 1; i < 3; ++i) {
		if (a[i] < min)
			min = a[i];
	}
	return min;
}

auto findmin(const std::array<double, 4> a) -> double {
	double min = a[0];
	for (int i = 1; i < 4; ++i) {
		if (a[i] < min)
			min = a[i];
	}
	return min;
}