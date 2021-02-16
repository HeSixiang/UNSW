// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <string>
#include <vector>

#include "benchmark/benchmark.h"
#include "range/v3/range/primitives.hpp"

auto make_a_very_long_string() -> std::string {
	return "The purpose of this benchmark is to compute the run-time a constant-time operation      "
	       "when passing an object by value, and to contrast that result against the run-time       "
	       "of the exact same constant-time operation when passing an object with the exact         "
	       "same value by reference-to-const. The lecture slides have diagrams and a recorded       "
	       "explanation, so you're better off finding out what's said over there. This is just      "
	       "the proof that the lecturer isn't trying to pull the wool over your eyes!               "
	       "                                                                                        "
	       "To understand what's going on, you only need to understand the following:               "
	       "                                                                                        "
	       "     1. `random_characters_by_value` is the name of an overload set that takes objects  "
	       "        whose types are `std::string` or `std::vector<std::string>` by value. Only the  "
	       "        interface is interesting, so you can minimise the function body and ignore it.  "
	       "                                                                                        "
	       "     2. `random_characters_by_reference` is the name of an overload set that takes      "
	       "        objects whose types are `std::string` or `std::vector<std::string>` by          "
	       "        reference-to-const. Only the interface is interesting, so you can minimise the  "
	       "        function body and ignore it.                                                    "
	       "                                                                                        "
	       "     3. `benchmark_passing_string_by_value` and `benchmark_passing_string_by_reference` "
	       "        respectively make calls to the above `random_characters_` functions, passing in "
	       "        this string; but they're otherwise identical. This is the **only** interesting  "
	       "        part about those benchmarking functions at this point in the course, so you're  "
	       "        free to minimise them and not care about the functions' contents one iota.      "
	       "                                                                                        "
	       "     4. `benchmark_passing_vector_of_strings_by_value` and                              "
	       "        `benchmark_passing_vector_of_strings_by_reference` respectively make calls to   "
	       "        the above `random_characters_` functions, passing in a vector containing a      "
	       "        number of copies of this string; otherwise they're identical to the functions in"
	       "        point #3. You can minimise them too.                                            "
	       "                                                                                        "
	       "     5. Instead, focus on running the program, and notice that when we pass a string by "
	       "        value, it takes significantly longer than when we pass it by                    "
	       "        reference-to-const.                                                            ";
}

namespace {
	// NOLINTNEXTLINE(performance-unnecessary-value-param)
	auto random_characters_by_value(std::string const sentence, int const index) -> char {
		auto const position = static_cast<std::string::size_type>(index) % ranges::size(sentence);
		return sentence[position];
	}

	auto random_characters_by_reference(std::string const& sentence, int const index) -> char {
		auto const position = static_cast<std::string::size_type>(index) % ranges::size(sentence);
		return sentence[position];
	}

	// NOLINTNEXTLINE(performance-unnecessary-value-param)
	auto random_characters_by_value(std::vector<std::string> const long_strings, int const index)
	   -> char {
		auto const position = static_cast<std::string::size_type>(index) % ranges::size(long_strings);
		return random_characters_by_reference(long_strings[position], index);
	}

	auto random_characters_by_reference(std::vector<std::string> const& long_strings, int const index)
	   -> char {
		auto const position = static_cast<std::string::size_type>(index) % ranges::size(long_strings);
		return random_characters_by_reference(long_strings[position], index);
	}

	void benchmark_passing_string_by_value(benchmark::State& state) {
		auto i = 0;
		auto str = make_a_very_long_string();
		for ([[maybe_unused]] auto _ : state) {
			[[maybe_unused]] auto result = '\0';
			benchmark::DoNotOptimize(str.data());
			benchmark::ClobberMemory();
			benchmark::DoNotOptimize(result = random_characters_by_value(str, i));
			++i;
		}
	}

	void benchmark_passing_string_by_reference(benchmark::State& state) {
		auto i = 0;
		auto str = make_a_very_long_string();
		for ([[maybe_unused]] auto _ : state) {
			[[maybe_unused]] auto result = '\0';
			benchmark::DoNotOptimize(str.data());
			benchmark::ClobberMemory();
			benchmark::DoNotOptimize(result = random_characters_by_reference(str, i));
			++i;
		}
	}

	void benchmark_passing_vector_of_strings_by_value(benchmark::State& state) {
		auto i = 0;
		auto const num_elements = 16;
		auto long_strings = std::vector<std::string>(num_elements, make_a_very_long_string());
		for ([[maybe_unused]] auto _ : state) {
			[[maybe_unused]] auto result = '\0';
			benchmark::DoNotOptimize(long_strings.data());
			benchmark::ClobberMemory();
			benchmark::DoNotOptimize(result = random_characters_by_value(long_strings, i));
			++i;
		}
	}

	void benchmark_passing_vector_of_strings_by_reference(benchmark::State& state) {
		auto i = 0;
		auto const num_elements = 16;
		auto long_strings = std::vector<std::string>(num_elements, make_a_very_long_string());
		for ([[maybe_unused]] auto _ : state) {
			[[maybe_unused]] auto result = '\0';
			benchmark::DoNotOptimize(long_strings.data());
			benchmark::ClobberMemory();
			benchmark::DoNotOptimize(result = random_characters_by_reference(long_strings, i));
			++i;
		}
	}
} // namespace

BENCHMARK(benchmark_passing_string_by_value); // NOLINT(cert-err58-cpp)
BENCHMARK(benchmark_passing_string_by_reference); // NOLINT(cert-err58-cpp)
BENCHMARK(benchmark_passing_vector_of_strings_by_value); // NOLINT(cert-err58-cpp)
BENCHMARK(benchmark_passing_vector_of_strings_by_reference); // NOLINT(cert-err58-cpp)

BENCHMARK_MAIN(); // NOLINT(cert-err58-cpp,modernize-use-trailing-return-type,readability-redundant-declaration)
