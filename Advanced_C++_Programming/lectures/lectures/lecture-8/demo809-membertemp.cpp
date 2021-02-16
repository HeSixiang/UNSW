#include <vector>

template<typename T>
class stack {
public:
	explicit stack() {}
	template<typename T2>
	stack(stack<T2>&);
	auto push(T t) -> void {
		stack_.push_back(t);
	}
	auto pop() -> T;
	auto empty() const -> bool {
		return stack_.empty();
	}

private:
	std::vector<T> stack_;
};

template<typename T>
T stack<T>::pop() {
	T t = stack_.back();
	stack_.pop_back();
	return t;
}

template<typename T>
template<typename T2>
stack<T>::stack(stack<T2>& s) {
	while (!s.empty()) {
		stack_.push_back(static_cast<T>(s.pop()));
	}
}

auto main() -> int {
	stack<int> is1;
	is1.push(2);
	is1.push(3);
	stack<int> is2{is1}; // this works
	stack<double> ds1{is1}; // this does not
}