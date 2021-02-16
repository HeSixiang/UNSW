class base_class {
protected:
	int i_;
};

class derived_class : public base_class {
public:
	auto foo(derived_class& o) -> void {
    	j_ = o.i_ + o.j_;
	}
private:
	int j_;
};