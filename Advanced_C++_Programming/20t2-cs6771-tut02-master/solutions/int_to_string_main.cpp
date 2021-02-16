#include <iostream>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include "int_to_string.hpp"

auto main() -> int {
	// Most of the explanation for what's going on with the pipelines can be found in
	// `int_to_string.cpp`.
	// In this specific situation, we're saying:
	//        Read in ints from the character input until something happens (see below),
	//        and then construct a vector of ints out of those integers.
	auto input = ranges::istream_view<int>(std::cin)
	           | ranges::to<std::vector>;

	// Streams have four states: good, eof, bad, and fail.
	//
	// * `good` means the stream can be read from; we didn't give a stop condition when building our
	//   range (e.g. read 10 ints), so we read until the stream was no longer good.
	// * `eof` means we've reached a sentinel that indicates we've reached the end-of-file. This can
	//   be a source of failure, but for our program, it's the state that we want.
	// * `bad` means something non-recoverable has happened, such as a disk being disconnected.
	// * `fail` means that we've encountered a recoverable error, like reading in a letter when we
	//   were expecting an int.
	//
	// Notice that we are checking for eof _after_ having finished reading: we don't check while
	// we're reading, but instead once we've detected something has gone wrong.
	if (not std::cin.eof()) {
		std::cerr << "Something went wrong, program exiting...\n";
		return 1;
	}

	std::cout << int_to_string(input) << '\n';
}
