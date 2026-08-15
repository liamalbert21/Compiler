set(LIB_TARGET_NAME "ErrorImpl")

add_library(${LIB_TARGET_NAME} STATIC
    Error_Lexer.cpp
    Error_Parser.cpp
)

target_link_libraries(${LIB_TARGET_NAME} PRIVATE PrimaryInterface)