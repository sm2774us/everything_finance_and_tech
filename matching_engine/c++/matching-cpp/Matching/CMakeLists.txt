add_library(matching INTERFACE)
target_sources(matching INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  matching
  INTERFACE
  CONAN_PKG::jemalloc
  CONAN_PKG::boost
  CONAN_PKG::spdlog
  CONAN_PKG::tbb
  CONAN_PKG::nlohmann_json
  CONAN_PKG::concurrentqueue
)

target_include_directories(
    matching
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/matching>
)
