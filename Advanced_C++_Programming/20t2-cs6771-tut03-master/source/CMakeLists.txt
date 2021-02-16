cxx_library(
   TARGET dict
   FILENAME "dict.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET dict_main
   FILENAME "dict_main.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_library(
   TARGET car
   FILENAME "car.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)
