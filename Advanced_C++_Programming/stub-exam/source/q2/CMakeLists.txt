cxx_library (
   TARGET "q2_lib"
   FILENAME "q2.cpp"
)

cxx_executable (
   TARGET "q2"
   FILENAME "client.cpp"
   LINK q2_lib
)
