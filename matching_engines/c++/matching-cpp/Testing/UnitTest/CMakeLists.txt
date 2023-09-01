add_executable(
  unit_tests
  main.cpp
)

target_link_libraries(
  unit_tests
  PRIVATE
  matching
  Threads::Threads
  CONAN_PKG::jemalloc
  CONAN_PKG::gtest
)

