set(COMPILER_CORE "CompilerCore")

add_library(${COMPILER_CORE} STATIC
    Expression.cpp
    Lexer.cpp
    Log.cpp
    Parser.cpp
    Token.cpp
)

target_compile_definitions(${COMPILER_CORE} PUBLIC
    PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_include_directories(${COMPILER_CORE} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:src/include>
)

add_subdirectory(errors)
target_link_libraries(${COMPILER_CORE} Errors)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(tests)
target_link_libraries(${PROJECT_NAME} PRIVATE CompilerCore)