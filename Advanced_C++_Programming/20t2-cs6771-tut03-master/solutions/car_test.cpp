#include <string>
#include <vector>

#include "catch2/catch.hpp"

#include "car.h"

TEST_CASE("car Checks") {
	car one;
	car two{"Toyota", 5};
	CHECK(car::get_num_cars() == 2);

	car three{two};
    CHECK(car::get_num_cars() == 3);

    three.set_num_seats(6);
    CHECK(three.get_num_seats() == 6);
}