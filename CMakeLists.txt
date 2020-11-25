cmake_minimum_required(VERSION 3.10)
project(tristan-thomas-assn5)

set(HEADER_FILES WordTree.hpp rlutil.h)
set(SOURCE_FILES WordTree.cpp)
set(UNIT_TEST_FILES TestWordTree.cpp)

add_executable(TypeAhead ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
add_executable(UnitTestRunner ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES})
set_property(TARGET TypeAhead PROPERTY CXX_STANDARD 17)
set_property(TARGET UnitTestRunner PROPERTY CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(TypeAhead PRIVATE /W4 /permissive-)
    target_compile_options(UnitTestRunner PRIVATE /W4 /permissive-)
    set_target_properties(TypeAhead PROPERTIES LINK_FLAGS "/STACK:10000000")
    set_target_properties(UnitTestRunner PROPERTIES LINK_FLAGS "/STACK:10000000")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(TypeAhead PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
    target_compile_options(UnitTestRunner PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
    target_compile_options(TypeAhead PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3)
    target_compile_options(UnitTestRunner PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000 -O3) 
endif()

find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES} main.cpp)
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()
    add_custom_target(
            ClangFormat
            COMMAND ${CLANG_FORMAT}
            -i
            -style=file
            ${SOURCE_FILES_PATHS})
else()
    message("Unable to find clang-format")
endif()

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include(ExternalProject)
ExternalProject_Add(googletest
 GIT_REPOSITORY https://github.com/google/googletest.git
 GIT_TAG master
 SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
 BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
 CONFIGURE_COMMAND ""
 BUILD_COMMAND ""
 INSTALL_COMMAND ""
 TEST_COMMAND ""
)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(UnitTestRunner gtest_main)
