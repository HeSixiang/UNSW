#include <vector>

class point {
  public:
    point(int x, int y) : x_(x), y_(y) {}

  private:
    int x_;
    int y_;
};

int main() {
  point p{1,2};
  double length = p;
}