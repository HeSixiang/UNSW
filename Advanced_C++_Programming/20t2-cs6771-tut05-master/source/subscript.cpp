#include <cassert>
#include <iostream>

class point {
  public:
    point(int x, int y) : x_(x), y_(y) {}
    [[nodiscard]] operator[](int i) -> int& {
      assert(i == 0 || i == 1);
      if (i == 0) return this->x_;
      else return this->y_;
    }

  private:
    int x_;
    int y_;
};

int main() {
  point q{99, -5};
  q[0] = -99;
  std::cout << q[0] << "\n";

  const point p{99, -5};
  std::cout << p[0] << "\n";
}