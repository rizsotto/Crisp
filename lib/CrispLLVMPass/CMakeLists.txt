
include(GNUInstallDirs)

generate_declarations(LLVMDeclarations.cpp init_llvm_base_types)

add_library(crispllvm SHARED
    LLVMCompilationInfo.cpp
    LLVMPrologPredicateRegistration.c
    LLVMPrologPredicates.cpp
    LLVMPrologQueries.cpp
    CrispModulePass.cpp)

add_dependencies(crispllvm LLVMDeclarations.inc)

include_directories(${CRISP_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${SWIPL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(${CLANG_DEFINITIONS})
add_definitions(${SWIPL_DEFINITIONS})

target_link_libraries(crispllvm PrologEngineWrapper ${SWIPL_LIBRARIES})

install(TARGETS crispllvm
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
