cmake_minimum_required (VERSION 2.8)

project (demoserver)

# binary output path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${demoserver_BINARY_DIR}")

# try enable c++11
# g++ and clang++ need flags to enable c++11
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" HAS_CPP11)
if (HAS_CPP11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# find saedb library
set(SAEDB_ROOT_PATH "../../saedb" CACHE FILEPATH "root of saedb library")

add_subdirectory (src)
add_subdirectory (${SAEDB_ROOT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/saedb)
