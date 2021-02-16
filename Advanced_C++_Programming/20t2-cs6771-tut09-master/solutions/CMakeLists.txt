cxx_executable(
   TARGET book_sol
   FILENAME "book_sol.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET type_sol
   FILENAME "type_sol.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET istream_sol
   FILENAME "istream_sol.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET subscript_sol
   FILENAME "subscript_sol.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)