#include <type_traits>

template <typename T>
struct is_real_number {
  static auto constexpr value = std::is_integral<T>::value || std::is_floating_point<T>::value;
}

// Alternative solution - the solution above won't compile in some instances
template <typename T>
using is_real_number = std::is_arithmetic<T>;