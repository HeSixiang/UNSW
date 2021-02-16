#include <iostream>
#include <vector>
#include <cmath>

class point {
  public:
    point(int x, int y) : x_(x), y_(y) {}
    explicit operator double() const {
      return sqrt(x_*x_ + y_*y_);
    }

  private:
    int x_;
    int y_;
};

int main() {
  auto const p = point{1,2};
  auto length = static_cast<double>(p);
  std::cout << length << "\n";
}