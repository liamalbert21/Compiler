#pragma once

#include "Pipeline.hpp"

#include <type_traits>

// Explicitly include Error.hpp to implement error functionality via Error<T>
// Critical errors should be handled by assertions/exceptions

// Here, a critical error is defined as some event that prohibits a stage in the
// pipeline from terminating (e.g. infinite recursion loop in the parser)

// Note: The class for which explicit template speciailization is provided must
// be a Subsystem
// i.e. "class Error<NotASubsystem> { ... };" will fail

template <typename T>
concept Subsystem = std::is_base_of_v<Pipeline, T>;

template <Subsystem S>
class Error;