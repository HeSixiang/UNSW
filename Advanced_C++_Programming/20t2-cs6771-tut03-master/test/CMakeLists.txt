cxx_library(
   TARGET test_main
   FILENAME test_main.cpp
   LINK Catch2::Catch2
)

cxx_test(
   TARGET lambda_string
   FILENAME "lambda_string.cpp"
   LINK range-v3
)
