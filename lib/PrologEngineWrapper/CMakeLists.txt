
add_library(PrologEngineWrapper STATIC
    ClangPrologQueries.cpp
    PrologUtilityFunctions.cpp
    RunPrologEngine.cpp)

include_directories(${CRISP_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${SWIPL_INCLUDE_DIRS})

add_definitions(${CLANG_DEFINITIONS})
add_definitions(${SWIPL_DEFINITIONS})


# for ubuntu, which is using cmake 2.8.7
function(set_position_independent_code_on target)
    message(STATUS "cmake version: ${CMAKE_VERSION}")
    if ((${CMAKE_MAJOR_VERSION} GREATER 2) OR
        ((${CMAKE_MAJOR_VERSION} EQUAL 2) AND (${CMAKE_MINOR_VERSION} GREATER 8)) OR
        ((${CMAKE_MAJOR_VERSION} EQUAL 2) AND (${CMAKE_MINOR_VERSION} EQUAL 8) AND (${CMAKE_PATCH_VERSION} GREATER 8)))
        set_target_properties(${target} PROPERTIES
            POSITION_INDEPENDENT_CODE True)
    else()
        set_target_properties(${target} PROPERTIES
            COMPILE_FLAGS -fPIC)
    endif()
endfunction()

set_position_independent_code_on(PrologEngineWrapper)
