cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET ev_constructors_test
   FILENAME "ev_constructors_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET ev_operations_test
   FILENAME "ev_operations_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET ev_member_function_test
   FILENAME "ev_member_function_test.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET ev_friends_test
   FILENAME "ev_friends_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET ev_utility_test
   FILENAME "ev_utility_test.cpp"
   LINK euclidean_vector
)
