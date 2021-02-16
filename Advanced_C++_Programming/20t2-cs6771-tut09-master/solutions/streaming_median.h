#include <concepts/concepts.hpp>
#include <iterator>
#include <set>
#include <stdexcept>
#include <type_traits>

template <typename T> requires (concepts::totally_ordered<T>)
class streaming_median {
 public:
  auto add(T value) -> void { data_.insert(std::move(value)); }

  auto median() const -> T {
    if (data_.empty()) {
      throw std::runtime_error{"Cannot get median of empty stream."};
    }

    if (data_.size() % 2 == 0) {
      if constexpr (std::is_arithmetic_v<T>) {
        auto hi = std::next(data_.begin(), data_.size() / 2);
        auto lo = std::prev(hi);
        return (*lo + *hi) / 2;
      } else {
        throw std::runtime_error{"Cannot get median of an evenly sized stream."};
      }
    }

    return *std::next(data_.begin(), data_.size() / 2);
  }

 private:
  std::multiset<T> data_;
};