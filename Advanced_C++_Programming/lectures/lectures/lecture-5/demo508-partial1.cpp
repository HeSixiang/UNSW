#include <exception>
#include <memory>
#include <stdexcept>

class my_int {
public:
	explicit my_int(int const i)
	: i_{i} {
		if (i == 2) {
			throw std::runtime_error("");
		}
	}

private:
	int i_;
};

class unsafe_class {
public:
	unsafe_class(int a, int b)
	// : a_{std::unique_ptr<my_int>(new my_int{a})}
	// , b_{std::unique_ptr<my_int>(new my_int{b})} {}
	: a_{std::make_unique<my_int>(a)}
	, b_{std::make_unique<my_int>(b)} {}

private:
	std::unique_ptr<my_int> a_;
	std::unique_ptr<my_int> b_;
};

int main() {
	auto a = unsafe_class(1, 2);
}