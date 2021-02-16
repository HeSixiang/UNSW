#include <vector>

class point {
  public:
    point(int x, int y) : x_(x), y_(y) {}
    operator==(const& point p1, const& point p2) -> bool {

    }
  private:
    int x_;
    int y_;
};

int main() {
  point p{1,2};
  point p2{2,3};
  double length = p;
}