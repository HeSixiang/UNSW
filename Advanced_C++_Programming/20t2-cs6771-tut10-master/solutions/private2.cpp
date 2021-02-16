class base_class {
public:
	auto get_i() -> int& {
		return i_;
	}
	auto get_i() const -> int {
		return i_;
	}
private:
	int i_;
};

class derived_class : public base_class {
public:
	auto foo(derived_class& o) -> void {
    	j_ = o.get_i() + o.j_;
	}
private:
	int j_;
};