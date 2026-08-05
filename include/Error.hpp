#pragma once

#include "Pipeline.hpp"

#include <concepts>

// Explicitly include Error.hpp to implement error functionality via Error<T>
// Critical errors should be handled by assertions/exceptions

// Here, a critical error is defined as some event that prohibits a stage in the
// pipeline from terminating (e.g. infinite recursion loop in the parser)

// Note: The class for which explicit template speciailization is provided must
// be a Subsystem
// i.e. "struct Error<NotASubsystem> { ... };" will fail

// Important: If any class were to inherit Pipeline privately with the
// intention of restricting T::State to its scope, the concept associated with
// Error<T> would fail

template <typename T>
concept Subsystem = std::derived_from<T, Pipeline>;

template <Subsystem S>
struct Error;