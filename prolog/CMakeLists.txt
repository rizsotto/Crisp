
include(GNUInstallDirs)

set(DATA_OBJ_ROOT ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

macro(generate_all_prolog_state)
  list(APPEND prolog_files "${CMAKE_CURRENT_SOURCE_DIR}/PrologBootForDeclExtractor/PrologBootForDeclExtractor.pl")
  list(APPEND prolog_files "${CMAKE_CURRENT_SOURCE_DIR}/PrologBootForCrispClangPlugin/PrologBootForCrispClangPlugin.pl")
  list(APPEND prolog_files "${CMAKE_CURRENT_SOURCE_DIR}/PrologBootForCrispLLVMPass/PrologBootForCrispLLVMPass.pl")

  foreach(prolog_file ${prolog_files})
    string(REGEX REPLACE "/(([^/]+)/)*([^/]+)\\.pl" "\\3.sh"
        shell_file ${prolog_file})
    add_custom_command(
        OUTPUT ${shell_file}
        DEPENDS ${prolog_file}
        COMMAND ${SWIPL_EXECUTABLE} --quiet -o ${shell_file} -c ${prolog_file})
    list(APPEND shell_files ${shell_file})
  endforeach()

  add_custom_target(generate_saved_state DEPENDS ${shell_files})
endmacro()

generate_all_prolog_state()

# do copy the Rule files to ${DATA_OBJ_ROOT} for test
configure_file(Rules/SomeHICPPrules.pl . COPYONLY)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PrologBootForCrispClangPlugin.sh
    ${CMAKE_CURRENT_BINARY_DIR}/PrologBootForCrispLLVMPass.sh
  DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")

install(
  DIRECTORY Rules/
  DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
  FILES_MATCHING PATTERN "*.pl")
