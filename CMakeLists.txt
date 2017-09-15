cmake_minimum_required(VERSION 3.5)
project(extractor C CXX)

set(CMAKE_CXX_STANDARD 11)
set(NAME extractor)

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3 -Ofast")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})


# Boost
find_package(Boost REQUIRED COMPONENTS filesystem system iostreams regex)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()


add_executable(mono mono.cpp)
target_link_libraries(mono ${Boost_LIBRARIES})
