#pragma once

/**
 * @brief Subsystems (pipelines) inherit this class privately to
 *           a) Access a state varaible without having to redefine it or provide
 *              a global entrypoint, and
 *           b) Acquire the ability to implement structured errors (see
 *              Error.hpp)
 *
 *        Include Pipeline.hpp if you only want access to the state variables
 *        for a particular pipeline, as Error.hpp already does so
 * 
 */
class Pipeline {
protected:
    enum class State {
        INIT, OKAY, FAIL
    };  
};