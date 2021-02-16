#include <exception>
#include <string>
#include <iostream>

class Logger {
public:
    auto log(const std::string &msg) -> void;
};

class DBConn {
public:
    auto try_connect(const std::string &uname, const std::string &pword) -> void;

private:
    bool active_ = false;
};

auto make_connection(const std::string &uname, const std::string &pword) -> DBConn {
    DBConn db;
    try {
        db.try_connect(uname, pword);
        return db;
    } catch (const std::string &e) {
        std::cout << e << std::endl;
        throw e;
    }
}

int main() {
    Logger l;

    try {
        make_connection("hsmith", "swagger/10");
    } catch (const std::string &e) {
        l.log("Could not establish connection");
    }
}