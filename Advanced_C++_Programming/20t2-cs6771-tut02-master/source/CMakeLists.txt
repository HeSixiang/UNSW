cxx_library(
   TARGET sort_descending
   FILENAME "sort_descending.cpp"
   LINK range-v3 gsl::gsl-lite-v1
)

cxx_library(
   TARGET all_unique
   FILENAME "all_unique.cpp"
   LINK absl::flat_hash_set range-v3 gsl::gsl-lite-v1
)

cxx_library(
   TARGET is_permutation
   FILENAME "is_permutation.cpp"
   LINK absl::flat_hash_map gsl::gsl-lite-v1
)

cxx_library(
   TARGET int_to_string_lib
   FILENAME "int_to_string.cpp"
   LIBRARY_TYPE STATIC
   LINK fmt::fmt-header-only absl::strings gsl::gsl-lite-v1 range-v3
)

cxx_executable(
   TARGET int_to_string
   FILENAME "int_to_string_main.cpp"
   LINK int_to_string_lib gsl::gsl-lite-v1 range-v3
)
