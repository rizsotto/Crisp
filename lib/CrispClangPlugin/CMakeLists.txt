
include(GNUInstallDirs)

generate_declarations(ClangDeclarations.cpp init_clang_base_types)

add_library(crispclang SHARED
    ClangPrologPredicateRegistration.c
    ClangPrologPredicates.cpp
    CompilationInfo.cpp
    CrispASTAction.cpp)

add_dependencies(crispclang ClangDeclarations.inc)

include_directories(${CRISP_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${SWIPL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${CLANG_DEFINITIONS})
add_definitions(${SWIPL_DEFINITIONS})

target_link_libraries(crispclang PrologEngineWrapper ${SWIPL_LIBRARIES})

install(TARGETS crispclang
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
