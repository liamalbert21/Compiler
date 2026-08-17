set(ERRORS "Errors")

add_library(${ERRORS} STATIC
    Error_Lexer.cpp
    Error_Parser.cpp
)

target_link_libraries(${ERRORS} PRIVATE CompilerCore)