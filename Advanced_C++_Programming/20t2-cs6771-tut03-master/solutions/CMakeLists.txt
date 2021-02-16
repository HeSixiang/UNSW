cxx_test(
   TARGET lambda_string_sol
   FILENAME "lambda_string.cpp"
)

cxx_test(
   TARGET car_test_sol
   FILENAME "car_test.cpp"
   LINK car_sol
)

cxx_test(
   TARGET dict_test_sol
   FILENAME "dict_test.cpp"
   LINK dict_sol
)

cxx_executable(
   TARGET dict_main_sol
   FILENAME "dict_main.cpp"
   LINK dict_sol
)

cxx_library(
   TARGET car_sol
   FILENAME "car.cpp"
)

cxx_library(
   TARGET dict_sol
   FILENAME "dict.cpp"
)
