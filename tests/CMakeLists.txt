cmake_minimum_required (VERSION 3.5)
project(test)


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(CPP_CACHE_VERSION_MAJOR 0)
set(CPP_CACHE_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(app_cppcache_test tests_cppcache.cpp)
add_executable(app_fileio_test tests_file_io.cpp)
target_link_libraries(app_cppcache_test ${GTEST_LIBRARIES} pthread )
target_link_libraries(app_fileio_test ${GTEST_LIBRARIES} pthread )

add_test(NAME app_cppcache_test
        COMMAND app_cppcache_test)

add_test(NAME app_fileio_test
        COMMAND app_fileio_test)