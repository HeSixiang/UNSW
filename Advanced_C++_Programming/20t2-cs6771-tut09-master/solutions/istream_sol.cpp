#include <ostream>
#include <istream>
#include <iostream>

class point {
  public:
    point(int x, int y) : x_{x}, y_{y} {};
    friend std::ostream& operator<<(std::ostream& os, point const& type); 
    friend std::istream& operator>>(std::istream& is, point& type); 

  private:
    int x_;
    int y_;
};

std::ostream& operator<<(std::ostream& os, point const& p) {
  os << "(" << p.x_ << "," << p.y_ << ")";
  return os;
}

std::istream& operator>>(std::istream& is, point& type) {
  return is >> p.x_ >> p.y_;
}

int main() {
  point p{1,2};
  std::cout << p << '\n';
}
