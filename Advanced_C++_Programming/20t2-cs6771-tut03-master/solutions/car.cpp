#include "./car.h"

int car::n_objects_ = 0;

car::~car() noexcept {
  --n_objects_;
}

auto car::get_manufacturer() const -> const std::string& {
  return manufacturer_;
}

auto car::get_num_seats() const -> int {
  return num_seats_;
}

auto car::set_num_seats(int num_seats) -> void {
  num_seats_ = num_seats;
}

auto car::get_num_cars() -> int {
  return n_objects_;
}
