#ifndef TUTORIALS_WEEK3_CAR_H_
#define TUTORIALS_WEEK3_CAR_H_

#include <string>

class car {
 public:
  car(): car{"unknown", 4} {}
  car(car const& c): car{c.manufacturer_, c.num_seats_} {}

  car(std::string manufacturer, int n_seats)
    : manufacturer_{ std::move(manufacturer) }, num_seats_{n_seats}
  {
    ++n_objects_;
  }

  ~car() noexcept;

  [[nodiscard]] auto get_manufacturer() const -> const std::string&;
  [[nodiscard]] auto get_num_seats() const -> int;
  void set_num_seats(int num_seats);

  static int get_num_cars();

 private:
  std::string manufacturer_;
  int num_seats_;

  static int n_objects_;
};

#endif  // TUTORIALS_WEEK3_CAR_H_
