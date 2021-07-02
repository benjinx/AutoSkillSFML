CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(AutoSkill)

###
### General Configuration
###

# Allow for custom organization of files in VisualStudio, Code::Blocks, etc
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

IF(MSVC)
    # Disable Default Exception Handling
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

###
### Options
###

OPTION(
    BUILD_THIRDPARTY
    "Build Missing ThirdParty Dependencies"
    ON
)

###
### Modules and Dependencies
###

SET(
    MODULES 
    "All" 
    CACHE STRING
    "Semicolon-separated list of required modules, use 'All' to build all modules"
)

# Generate list of required third party dependencies
SET(REQUIRED_DEPENDENCIES
    "SFML"
)

IF("SFML" IN_LIST REQUIRED_MODULES)
    LIST(APPEND REQUIRED_MODULES "SFML")
ENDIF()


LIST(REMOVE_DUPLICATES REQUIRED_MODULES)
LIST(REMOVE_DUPLICATES REQUIRED_DEPENDENCIES)

MESSAGE(STATUS "Available modules: ${AVAILABLE_MODULES}")
MESSAGE(STATUS "Building Modules: ${REQUIRED_MODULES}")
MESSAGE(STATUS "Required Dependencies: ${REQUIRED_DEPENDENCIES}")

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

    # Set _ROOT variables for config-based packages
    SET(SFML_ROOT  ${THIRDPARTY_BINARY_DIR}/SFML)

    # CMake refuses to quote the list when passing it on the command line, and `;` will
    # split the command in half, so we have to switch them to a different separator
    STRING(REPLACE ";" ":" _tmp "${REQUIRED_DEPENDENCIES}")
    
    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DREQUIRED_DEPENDENCIES=${_tmp}
            ${THIRDPARTY_SOURCE_DIR}
            WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure thirdparty projects")
    ENDIF()

    SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_COMMAND} --build . )

    # If we're using `make`, we can build with extra cores
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_THIRDPARTY_BUILD_COMMAND}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build thirdparty projects")
    ENDIF()
ENDIF()

SET(SFML_STATIC_LIBRARIES ON)

FIND_PACKAGE(SFML CONFIG COMPONENTS system window graphics REQUIRED)

###
### 
###

FILE(GLOB_RECURSE
    _sources
    "src/*.h"
    "src/*.hpp"
    "src/*.c"
    "src/*.cpp"
)

###
### Target Configuration
###

INCLUDE(SetSourceGroups)

ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${_sources}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")

TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/src
)

TARGET_COMPILE_DEFINITIONS(
    ${PROJECT_NAME}
    PUBLIC
        # Disable VS "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
        SFML_STATIC
)

TARGET_COMPILE_OPTIONS(
    ${PROJECT_NAME}
    PRIVATE
        # Configure VS to use C++20, since it ignores CXX_STANDARD
        $<$<CXX_COMPILER_ID:MSVC>: /std:c++latest>

        # Force windows to use UTF-8
        $<$<CXX_COMPILER_ID:MSVC>: /utf-8>

        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:MSVC>:  /MP /wd4068 /EHsc->
)

SET_TARGET_PROPERTIES(
    ${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
SET_TARGET_PROPERTIES(
    ${PROJECT_NAME}
    PROPERTIES
        FOLDER "${folder}"
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    PRIVATE
        SFML::SFML
)