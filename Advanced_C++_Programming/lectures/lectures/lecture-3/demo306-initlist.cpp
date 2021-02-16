#include <string>

class nodefault {
public:
	explicit nodefault(int i)
	: i_{i} {}

private:
	int i_;
};

int const b_default = 5;

class b {
	// Constructs s_ with value "Hello world"
	explicit b(int& i)
	: s_{"Hello world"}
	, const_{b_default}
	, no_default_{i}
	, ref_{i} {}

	// Doesn't work - constructed in order of member declaration.
	/*explicit b(int& i)
	: s_{"Hello world"}
	, const_{5}
	, ref_{i}
	, no_default_{ref_} {}*/

	/*explicit b(int& i) {
	   // Constructs s_ with an empty string, then reassigns it to "Hello world"
	   // Extra work done (but may be optimised out).
	   s_ = "Hello world";

	   // Fails to compile (modifying a const object).
	   const_string_ = "Goodbye world";
	   // Fails to compile (references *must* be initialized in the constructor).
	   ref_ = i;
	   // This is fine, but it can't construct it initially.
	   no_default_ = nodefault{1};
	}*/

	std::string s_;
	// All of these will break compilation if you attempt to put them in the body.
	const int const_;
	nodefault no_default_;
	int& ref_;
};