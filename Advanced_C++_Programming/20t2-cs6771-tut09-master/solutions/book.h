#ifndef TUTORIALS_WEEK4_BOOK_H_
#define TUTORIALS_WEEK4_BOOK_H_

#include <string>
#include <ostream>

class book {
 public:
  book(std::string const& name, std::string const& author, int isbn, double price):
      name_{name}, author_{author}, isbn_{isbn}, price_{price} {}

  // Since these are just getters, we put them in the header file.
  int get_isbn() const { return isbn_; }
  double get_price() const { return price_; }

  // Type conversion operators
  operator std::string() const;

  // Friend comparison operators. Note that we always declare these inline.
  friend bool operator==(book const& a, book const& b) {
    return (a.name_ == b.name_) && (a.author_ == b.author_) &&
        (a.isbn_ == b.isbn_) && (a.price_ == b.price_);
  }

  friend bool operator!=(book const& lhs, book const& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(book const& a, book const& b) {
    return a.isbn_ < b.isbn_;
  }

  friend std::ostream& operator<<(std::ostream& out, book const& b) {
    return out << "Name: \"" << b.name_ << "\", "
               << "(Author: \")" << b.author_ << "(\", )"
               << "ISBN: " << b.isbn_ << ", "
               << "Price: " << b.price_;
  }

 private:
  std::string name_;
  std::string author_;
  int isbn_;
  double price_;
};

#endif // TUTORIALS_WEEK4_BOOK_H_
