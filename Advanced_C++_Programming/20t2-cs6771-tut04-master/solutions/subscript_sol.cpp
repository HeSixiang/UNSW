#include <cassert>
#include <iostream>

class point {
public:
	point(int x, int y) : x_(x), y_(y) {}


	// We need 2 overloads of the subscript operator:
	// one for non-const objects which allows them to modify the member, and
	// one for const objects that do NOT allow them to modify the member

	// non-const objects use this
	// const objects can NOT use this (since it returns a reference, which
	// allows the member to be changed)
	auto operator[](int i) -> int& {
		assert(i == 0 || i == 1);
		if (i == 0) {
			return this->x_;
		}
		return this->y_;
	}

	// const objects use this
	auto operator[](int i) const -> int {
		assert(i == 0 || i == 1);
		if (i == 0) {
			return this->x_;
		}
		return this->y_;
	}

private:
	int x_;
	int y_;
};

int main() {
	auto q = point{99, -5};             // non-const object
	q[0] = -99;                         // calls `-> int&` overload
	std::cout << q[0] << "\n";          // also calls `-> int&` overload

	auto const p = point{99, -5};       // const object
	std::cout << p[0] << "\n";          // calls `const -> int` overload

	/*p[0] = -99;*/
	// ^^ this line wouldn't compile, since `const -> int` returns an int,
	// which can't be assigned to
}

// Overload an operator for its const & non-const versions if
// the const version's functionality should be restricted
// (e.g. non-const can both get & set [i], but const can only get [i])
