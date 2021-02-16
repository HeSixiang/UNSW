#include <vector>

class point {
  public:
    point(int x, int y) : x_(x), y_(y) {}
    operator double() {
      return sqrt(x_*x_ + y_*y_);
    }

  private:
    int x_;
    int y_;
};

int main() {
  point p{1,2};
  double length = p;
}