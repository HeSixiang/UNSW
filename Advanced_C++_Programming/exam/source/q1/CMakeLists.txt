cxx_library (
   TARGET "q1"
   FILENAME "q1.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable (
   TARGET "q1-example-usage"
   FILENAME "q1-example-usage.cpp"
   LINK q1
)
