#include <iostream>

class Giraffe {
public:
	Giraffe() {
		std::cout << "Giraffe constructed" << '\n';
	}
	Giraffe(const Giraffe& g) {
		std::cout << "Giraffe copy-constructed" << '\n';
	}
	~Giraffe() {
		std::cout << "Giraffe destructed" << '\n';
	}
};

void zebra() {
	throw Giraffe{};
	std::cout << "This won't print!"
	          << "\n";
}

void llama() {
	try {
		zebra();
	} catch (Giraffe const& g) {
		std::cout << "caught in llama; rethrow" << '\n';
		throw;
	}
}

int main() {
	try {
		llama();
	} catch (Giraffe const& g) {
		std::cout << "caught in main" << '\n';
	}
}