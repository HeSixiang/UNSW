cxx_library(
   TARGET sort_descending_solution
   FILENAME "sort_descending.cpp"
   LINK range-v3
)

cxx_test(
   TARGET sort_descending_solution_test
   FILENAME "sort_descending_test.cpp"
   LINK
      sort_descending_solution # We link this, as it's the library we're testing
      range-v3                 # We link this, as we use range-v3 in the test.
)

cxx_library(
   TARGET all_unique_solution
   FILENAME "all_unique.cpp"
   LINK absl::flat_hash_set range-v3
)

cxx_library(
   TARGET is_permutation_solution
   FILENAME "is_permutation.cpp"
   LINK absl::flat_hash_map
)

cxx_library(
   TARGET solution_int_to_string
   FILENAME "int_to_string.cpp"
   LIBRARY_TYPE STATIC        # This tells CMake that we plan to link to an executable as a static
   LINK                       # library.
      absl::strings
      fmt::fmt-header-only
      range-v3
)

cxx_executable(
   TARGET solution_int_to_string_main
   FILENAME "int_to_string_main.cpp"
   LINK solution_int_to_string
)

cxx_test(
   TARGET solution_int_to_string_test
   FILENAME "int_to_string_test.cpp"
   LINK solution_int_to_string
)
