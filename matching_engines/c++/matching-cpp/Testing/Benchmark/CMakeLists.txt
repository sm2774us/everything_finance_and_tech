add_executable(
    benchmark
    main.cpp
)

target_link_libraries(
    benchmark
    PRIVATE
    matching
    CONAN_PKG::benchmark
    CONAN_PKG::jemalloc
)

