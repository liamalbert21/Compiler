set(TESTS "Tests")

add_executable(${TESTS} Tests.cpp)

find_package(Catch2 3 REQUIRED)

target_link_libraries(${TESTS} PRIVATE CompilerCore Catch2::Catch2WithMain)