#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../source/dict.h"

auto main() -> int{
  std::ifstream word_list_file = std::ifstream{"path/to/text/file"};
  print_valid_words(to_word_list(word_list_file), std::cin, std::cout);
}