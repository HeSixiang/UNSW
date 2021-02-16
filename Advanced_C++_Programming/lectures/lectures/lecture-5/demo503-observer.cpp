#include <iostream>
#include <memory>

auto main() -> int {
	auto* heap_resource = new int{0};

	auto up1 = std::unique_ptr<int>(heap_resource);
	auto const& up2 = up1;
	(void)up2;
}