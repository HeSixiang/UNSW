#include <iostream>
#include <memory>

auto main() -> int {
	auto* my_heap_object = new std::string{"Hi"};
	auto up1 = std::unique_ptr<std::string>(my_heap_object);

	// auto up2 = up1; DON'T LIKE THIS

	auto up3 = std::unique_ptr<std::string>(new std::string{"Goodbye"});
	up3 = std::move(up1);

	std::cout << "What is this??? " << *up1 << "\n";

	// What the hell is up1 doing now?!?!

	// up3 = up2; // no copy assignment DON'T LIKE THIS

	// up3.reset(up1.release()); // OK
	// auto up4 = std::move(up3); // OK
	// std::cout << up4.get() << "\n";
	// std::cout << *up4 << "\n";

	// std::cout << *up1 << "\n";
	// std::cout << *(up1.get()) << "\n";
	// std::cout << up1 << "\n";
	// std::cout << up1.get() << "\n";
	// std::cout << up1->size() << "\n";
	// std::cout << *my_heap_object << "\n";
	// std::cout << my_heap_object->size() << "\n";
}