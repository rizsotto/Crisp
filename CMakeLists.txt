
cmake_minimum_required(VERSION 2.8)

project(Crisp C CXX)

set(CPACK_PACKAGE_NAME "Crisp")
set(CPACK_PACKAGE_CONTACT "Guillem Marpons <gmarpons@babel.ls.fi.upm.es>")
set(CPACK_PACKAGE_VENDOR ${CPACK_PACKAGE_CONTACT})
set(CPACK_PACKAGE_VERSION "0.2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Clang/LLVM add-on to enforce coding rules")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_GENERATOR "TGZ;RPM;DEB")
set(CPACK_RPM_PACKAGE_RELEASE "1%{?dist}")
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_PACKAGE_URL "http://github.com/gmarpons/Crisp")
set(CPACK_RPM_PACKAGE_DESCRIPTION "Crisp rules are written in Prolog. A high-level declarative DSL to easily write new rules is under development. It will be called CRISP, an acronym for Coding Rules in Sugared Prolog.")
include(CPack)

include(GNUInstallDirs)
install(FILES COPYING README.md
  DESTINATION ${CMAKE_INSTALL_DOCDIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)
find_package(Swipl REQUIRED)
find_package(Clang REQUIRED)

list(APPEND include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND include_dirs ${CMAKE_CURRENT_BINARY_DIR}/include)
set(CRISP_INCLUDE_DIRS ${include_dirs})

set(CMAKE_CXX_COMPILER "${CLANG_EXECUTABLE}")
set(CMAKE_C_COMPILER "${CLANG_EXECUTABLE}")

add_definitions("-DDATA_INSTALL_ROOT=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")

add_subdirectory(include)
add_subdirectory(prolog)
# order is important!!! lib is using variable from prolog.
add_subdirectory(lib)
add_subdirectory(test)
