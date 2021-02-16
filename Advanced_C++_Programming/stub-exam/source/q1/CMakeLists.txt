cxx_library (
   TARGET "q1_lib"
   FILENAME "q1.cpp"
)

cxx_executable (
   TARGET "q1"
   FILENAME "client.cpp"
   LINK q1_lib
)
