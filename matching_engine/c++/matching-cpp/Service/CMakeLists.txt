add_library(service ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(service PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
  matching_service
  src/main.cpp
)

target_link_libraries(
  matching_service
  PRIVATE
  matching
  CONAN_PKG::jemalloc
)

set_target_properties(service PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
  service
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/service>
)
