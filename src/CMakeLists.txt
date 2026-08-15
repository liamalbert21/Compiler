add_executable(${PROJECT_NAME}
    Expression.cpp
    Lexer.cpp
    Log.cpp
    main.cpp
    Parser.cpp
    Token.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_WARNING_AS_ERROR ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Eventual GUI shouldn't need access to 
add_library(PrimaryInterface INTERFACE)
target_include_directories(PrimaryInterface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/include>
    $<INSTALL_INTERFACE:src/include>
)

add_subdirectory(errors)

target_link_libraries(${PROJECT_NAME} PUBLIC
    PrimaryInterface
    ErrorImpl
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)