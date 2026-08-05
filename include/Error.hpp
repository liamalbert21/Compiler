#pragma once

// Explicitly include Error.hpp to implement error functionality via Error<T>
// Critical errors should be handled by assertions/exceptions

// Here, a critical error is defined as some event that causes
// a stage in the pipelien to not terminate (e.g. infinite recursion
// loop in the parser).

template <typename T>
struct Error;