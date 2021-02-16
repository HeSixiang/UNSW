#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <typename T, typename... Args>
auto my_make_unique(Args&&... args) {
  return std::unique_ptr<T>{new T{std::forward<Args>(args)...}};
}

auto main() -> int {
  myclass myclass{"myclass"};
  std::cout << *my_make_unique<myclass>(myclass) << "\n";
  std::cout << *my_make_unique<myclass>(std::move(myclass)) << "\n";
  NonCopyable nonCopyable{"NonCopyable"};
  std::cout << *my_make_unique<NonCopyable>(std::move(nonCopyable)) << "\n\n";

  // Use the size constructor.
  std::cout << my_make_unique<std::vector<myclass>>(5U)->size() << "\n";

  // Use the size-and-value constructor.
  myclass base{"hello"};
  std::cout << my_make_unique<std::vector<myclass>>(6U, base)->size() << "\n";
}