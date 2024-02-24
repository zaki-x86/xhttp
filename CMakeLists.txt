cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0069 NEW)

project(xhttp VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts ${CMAKE_MODULE_PATH})

option(XHttpEnableTesting "Enable unit tests for the projects (from the test subfolder)." ON)
option(XHttpBuildExamples "Enable unit tests for the projects (from the test subfolder)." ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################################################################################
# Get dependencies
##################################################################################
# include(ExternalProject)
# find_package(Git REQUIRED)

# ExternalProject_Add(
#     doctest
#     PREFIX ${CMAKE_BINARY_DIR}/doctest
#     GIT_REPOSITORY https://github.com/doctest/doctest.git
#     TIMEOUT 10
#     UPDATE_COMMAND ${GIT_EXECUTABLE} pull
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     INSTALL_COMMAND ""
#     LOG_DOWNLOAD ON
# )

# # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
# ExternalProject_Get_Property(doctest source_dir)
# set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")
# message(STATUS "Doctest path is set to ${DOCTEST_INCLUDE_DIR}")

add_subdirectory(xhttp)

if(XHttpEnableTesting)
  enable_testing()
  add_subdirectory(test)
endif()

if(XHttpBuildExamples)
  add_subdirectory(examples/demo)
#  add_subdirectory(examples/ex2)
endif()
