cxx_library(
   TARGET test_main
   FILENAME test_main.cpp
   LINK Catch2::Catch2
)

cxx_test(
   TARGET sort_descending_test
   FILENAME "sort_descending_test.cpp"
   LINK range-v3 sort_descending
)

cxx_test(
   TARGET all_unique_test
   FILENAME "all_unique_test.cpp"
   LINK all_unique
)

cxx_test(
   TARGET is_permutation_test
   FILENAME "is_permutation_test.cpp"
   LINK is_permutation
)

cxx_test(
   TARGET int_to_string_test
   FILENAME "int_to_string_test.cpp"
   LINK int_to_string_lib
)
