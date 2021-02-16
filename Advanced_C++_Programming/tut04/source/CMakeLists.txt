cxx_executable(
   TARGET book
   FILENAME "book.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET type
   FILENAME "type.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET istream
   FILENAME "istream.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_executable(
   TARGET subscript
   FILENAME "subscript.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)