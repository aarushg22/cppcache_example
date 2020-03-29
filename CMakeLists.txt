cmake_minimum_required (VERSION 3.5)
project(cppcache)

set(CPP_CACHE_VERSION_MAJOR 0)
set(CPP_CACHE_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(cache_lib ${PROJECT_SOURCE_DIR}/src/cppcache.cpp)
target_include_directories(cache_lib
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)
add_executable(app ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(app cache_lib)

# enable_testing()

# add_subdirectory(test)