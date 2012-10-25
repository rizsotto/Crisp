
set(DECLEXTRACTOR_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

add_library(declextractor SHARED
    DeclExtractorASTAction.cpp
    DeclExtractorPrologPredicateRegistration.c
    DeclExtractorPrologPredicates.cpp)

include_directories(${CRISP_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${SWIPL_INCLUDE_DIRS})

add_definitions(${CLANG_DEFINITIONS})
add_definitions(${SWIPL_DEFINITIONS})

target_link_libraries(declextractor PrologEngineWrapper ${SWIPL_LIBRARIES})


function(generate_declarations input_file method)
  # prepare compiler flags for clang
  list(APPEND cxxflags ${CLANG_DEFINITIONS})
  foreach(include_dir ${CLANG_INCLUDE_DIRS})
    list(APPEND cxxflags -I${include_dir})
  endforeach()
  # prepare compiler flags for decl extractor plugin
  list(APPEND declflags -load ${DECLEXTRACTOR_BUILD_DIR}/libdeclextractor.so)
  list(APPEND declflags -plugin decl-extractor)
  list(APPEND declflags -plugin-arg-decl-extractor ${DATA_OBJ_ROOT})
  list(APPEND declflags -plugin-arg-decl-extractor ${method})
  foreach(flag ${declflags})
    list(APPEND cxxflags -Xclang ${flag})
  endforeach()
  # generate output file name
  string(REGEX REPLACE "([^/]+)\\.cpp" "\\1.inc"
        output_file ${input_file})
  # generate full path file names
  set(input ${CMAKE_CURRENT_SOURCE_DIR}/${input_file})
  set(output ${CMAKE_CURRENT_BINARY_DIR}/${output_file})
  # run the decl extractor pugin
  add_custom_command(
    OUTPUT ${output}
    COMMAND ${CLANG_EXECUTABLE} -x c++ -c ${cxxflags} ${input}
    DEPENDS declextractor generate_saved_state ${input})
  # create target which will pull it in
  add_custom_target(${output_file} ALL DEPENDS ${output})
endfunction()
