cmake_minimum_required(VERSION 3.5)
project(extractor C CXX)

set(CMAKE_CXX_STANDARD 11)
set(NAME extractor)

# options
option(BUILD_TEST "Build tests" ON)
message("BUILD_TEST: " ${BUILD_TEST})

# flags
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3 -Ofast")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})


# Boost
find_package(Boost REQUIRED COMPONENTS filesystem system iostreams regex)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

# gtest
enable_testing()
find_package(GTest REQUIRED)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif ()

# update submodules
execute_process(COMMAND git submodule update --init -- cld2
                WORKING_DIRECTORY src/3rd_party)

add_executable(mono src/mono/mono.cpp)
target_link_libraries(mono ${Boost_LIBRARIES})

add_executable(readwarc src/mono/readwarc.cpp)
target_link_libraries(readwarc ${Boost_LIBRARIES})


# build tests
if (BUILD_TEST)
    add_subdirectory(tests)
endif (BUILD_TEST)
