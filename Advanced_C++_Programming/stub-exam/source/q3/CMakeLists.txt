cxx_library (
   TARGET "q3_lib"
   FILENAME "q3.cpp"
)

cxx_executable (
   TARGET "q3"
   FILENAME "client.cpp"
   LINK q3_lib
)
