#include <iterator>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <exception>

class permission_error : public std::exception{};

// Defined elsewhere
auto exec_cmd(const std::string &cmd, const std::vector<std::string> &args) -> void;

int main() {
    auto cmd = std::vector<std::string>();
    while (true) {
        std::copy(
            std::istream_iterator<std::string>(std::cin),
            std::istream_iterator<std::string>(),
            std::back_inserter(cmd)
        );

        try {
            exec_cmd(cmd.front(), cmd);
        } catch (const permission_error &e) {
            std::cout << "Cannot exec: " << cmd.front() << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cout << "Invalid argument to: " << cmd.front() << std::endl;
        }
    }
}