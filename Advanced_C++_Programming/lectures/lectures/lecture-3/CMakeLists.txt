cxx_library(
   TARGET demo300-bookstore
   FILENAME "demo300-bookstore.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo300-bookstore_main
   FILENAME "demo300-bookstore_main.cpp"
   LINK absl::strings demo300-bookstore
)

cxx_executable(
   TARGET demo301-scope
   FILENAME "demo301-scope.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo302-construction
   FILENAME "demo302-construction.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo303-construction2
   FILENAME "demo303-construction2.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo304-classbasic
   FILENAME "demo304-classbasic.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo306-initlist
   FILENAME "demo306-initlist.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo307-deleg
   FILENAME "demo307-deleg.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo308-const
   FILENAME "demo308-const.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo309-static
   FILENAME "demo309-static.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo310-explicit1
   FILENAME "demo310-explicit1.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo310-explicit2
   FILENAME "demo310-explicit2.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo311-delete
   FILENAME "demo311-delete.cpp"
   LINK absl::strings
)

cxx_executable(
   TARGET demo351-point1
   FILENAME "demo351-point1.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo352-point2
   FILENAME "demo352-point2.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo353-io
   FILENAME "demo353-io.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo354-compassign
   FILENAME "demo354-compassign.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo355-relation1
   FILENAME "demo355-relation1.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo358-incdec
   FILENAME "demo358-incdec.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo359-arrow
   FILENAME "demo359-arrow.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo360-type
   FILENAME "demo360-type.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)

cxx_executable(
   TARGET demo361-syntax
   FILENAME "demo361-syntax.cpp"
   LINK absl::strings fmt::fmt-header-only range-v3
)
