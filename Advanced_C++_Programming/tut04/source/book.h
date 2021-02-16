#include <string>

class book {
public:

	[[nodiscard]]auto get_isbn() const -> int {
		return isbn_;
	}
	[[nodiscard]]auto get_price() const -> double {
		return price_;
	}

private:
	std::string name_;
	std::string author_;
	int isbn_;
	double price_;
};