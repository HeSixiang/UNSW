#include <iostream>
#include <set>
#include <unordered_set>

template <typename T, typename Container = std::set<T>>
class silly_set {
public:
	void insert(T const& t);
	bool erase(T const& t);

	friend std::ostream& operator<<(std::ostream& os, silly_set s) {
		for (const auto& i : s.set_) {
		  os << i << " ";
		}
		return os;
	}
private:
	Container set_;
};

template <typename T, typename Container>
void silly_set<T, Container>::insert(T const& t) {
	set_.insert(t);
}

template <typename T, typename Container>
bool silly_set<T, Container>::erase(T const& t) {
	auto it = set_.find(t);
	if (it == set_.end()) {
		return false;
	}

	set_.erase(it);
	return true;
}

int main() {
	auto s1 = silly_set<double>();
	s1.insert(5.4);
	s1.insert(6.2);
	s1.erase(5.4);
	std::cout << s1 << '\n';

	auto s2 = silly_set<int>();
	s2.insert(5);
	s2.insert(6);
	s2.erase(5);

	std::cout << s2 << '\n';
}
