#include "./book.h"

#include <algorithm>
#include <iostream>
#include <vector>

// Helper function to print vectors of books.
void print(const std::string& title, const std::vector<book>& books) {
  std::cout << title << '\n';
  for (const auto& b: books) {
    std::cout << "\t";
    std::cout << b << '\n';
  }
  std::cout << '\n';
}

int main() {
  std::vector<book> books1{
      book{"book1", "Author1", 2222, 12.2},
      book{"book2", "Author2", 1111, 11.50},
      book{"book3", "Author3", 3333, 10.50}};

  std::vector<book> books2{
      book{"book1", "Author1", 2222, 12.2},
      book{"book2", "Author2", 1111, 11.50},
      book{"book3", "Author3", 3333, 10.50}};

  print("books:", books1);

  // Call the std::vector<book> operator==() overload which in
  // turn calls book operator==() overload.
  std::cout << "Vector of books are equal: "
            << "books1 " << (books1 == books2 ? "==" : "!=")
            << " books2\n\n";

  std::sort(books1.begin(), books1.end());
  print("Default (ISBN) sorted books:", books1);

  std::cout << "Vector of books are no longer equal: "
            << "books1 " << (books1 == books2 ? "==" : "!=")
            << " books2\n\n";

  std::sort(books1.begin(), books1.end(),
            [](const book& a, const book& b) {
              return a.getPrice() < b.getPrice();
            }
  );

  print("Price sorted books:", books1);
}
