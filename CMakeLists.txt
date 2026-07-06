cmake_minimum_required(VERSION 4.3.3)
project(Lexer LANGUAGES CXX VERSION 1.0)
add_executable(${PROJECT_NAME}
    src/Lexer.cpp
    src/main.cpp
    src/Token.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)