CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(thirdparty)

SET(REQUIRED_DEPENDENCIES "" CACHE STRING "")

# See /CMakeLists.txt for explanation
STRING(REPLACE ":" ";" REQUIRED_DEPENDENCIES "${REQUIRED_DEPENDENCIES}")

# SFML

SET(SFML_ROOT       ${CMAKE_BINARY_DIR}/SFML)
SET(SFML_SOURCE_DIR ${CMAKE_BINARY_DIR}/SFML_src)
SET(SFML_BINARY_DIR ${CMAKE_BINARY_DIR}/SFML_bin)

SET(SFML_STATIC_LIBRARIES ON)

FIND_PACKAGE(SFML CONFIG COMPONENTS system window graphics QUIET)

IF(NOT SFML_FOUND OR NOT TARGET SFML)
    ExternalProject_Add(
        thirdparty-SFML
        URL                 "https://github.com/SFML/SFML/archive/refs/tags/2.5.1.zip"
        SOURCE_DIR          ${SFML_SOURCE_DIR}
        BINARY_DIR          ${SFML_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SFML_ROOT}
                            -DBUILD_SHARED_LIBS=OFF
        UPDATE_COMMAND      ""
        BUILD_COMMAND       ""
        TEST_COMMAND        ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        COMMAND             ${CMAKE_COMMAND} --build . --target install --config Debug
    )
ENDIF()