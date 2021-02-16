cxx_test(
   TARGET demo100-hello
   FILENAME "demo100-hello.cpp"
)

cxx_test(
   TARGET demo101-types
   FILENAME "demo101-types.cpp"
)

cxx_test(
   TARGET demo102-expressions
   FILENAME "demo102-expressions.cpp"
   LINK absl::strings
)

cxx_test(
   TARGET demo103-conversions
   FILENAME "demo103-conversions.cpp"
   LINK gsl::gsl-lite-v1
)

cxx_test(
   TARGET demo104-functions
   FILENAME "demo104-functions.cpp"
)

cxx_test(
   TARGET demo105-selection
   FILENAME "demo105-selection.cpp"
)

cxx_test(
   TARGET demo106-vector
   FILENAME "demo106-vector.cpp"
   LINK range-v3
)

cxx_benchmark(
   TARGET demo107-values
   FILENAME "demo107-values.cpp"
)

cxx_test(
   TARGET demo108-iteration
   FILENAME "demo108-iteration.cpp"
)

cxx_test(
   TARGET demo109-enum
   FILENAME "demo109-enum.cpp"
)

cxx_test(
   TARGET demo110-struct
   FILENAME "demo110-struct.cpp"
   LINK range-v3
)

cxx_test(
   TARGET demo111-flat_hash_set
   FILENAME "demo111-flat_hash_set.cpp"
   LINK absl::flat_hash_set
)

cxx_test(
   TARGET demo112-flat_hash_map
   FILENAME "demo112-flat_hash_map.cpp"
   LINK absl::flat_hash_map
)
