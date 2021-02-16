cxx_executable(
   TARGET exception1
   FILENAME "exception1.cpp"
)

cxx_executable(
   TARGET exception2
   FILENAME "exception2.cpp"
   LINK fmt::fmt-header-only
)

cxx_executable(
   TARGET exception_bad
   FILENAME "exception_bad.cpp"
)

cxx_executable(
   TARGET exception_good
   FILENAME "exception_good.cpp"
)

cxx_executable(
   TARGET catchvalue
   FILENAME "catchvalue.cpp"
)

cxx_executable(
   TARGET catchreference
   FILENAME "catchreference.cpp"
)
