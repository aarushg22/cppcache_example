cmake_minimum_required (VERSION 3.5)
project(cppcache)


find_package (Threads)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(CPP_CACHE_VERSION_MAJOR 0)
set(CPP_CACHE_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(app ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries (app ${CMAKE_THREAD_LIBS_INIT})

# Testing
enable_testing()

add_subdirectory(tests)