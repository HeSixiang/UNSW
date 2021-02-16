cxx_executable(
   TARGET add_numbers_solution
   FILENAME "add_numbers.cpp"
)

cxx_executable(
   TARGET cat_solution
   FILENAME "cat.cpp"
)

cxx_test(
   TARGET sort3_solution
   FILENAME "sort3.cpp"
   LINK range-v3
)
