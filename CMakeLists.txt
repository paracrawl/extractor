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

# cld2
set(CLD2_DIR "src/3rd_party/cld2")
set(CLD2_DIR_INT "src/3rd_party/cld2/internal")
include_directories("${CLD2_DIR}/internal")
include_directories("${CLD2_DIR}/public")
set(libcld2_full_files ${CLD2_DIR_INT}/cldutil.cc ${CLD2_DIR_INT}/cldutil_shared.cc ${CLD2_DIR_INT}/compact_lang_det.cc ${CLD2_DIR_INT}/compact_lang_det_hint_code.cc ${CLD2_DIR_INT}/compact_lang_det_impl.cc ${CLD2_DIR_INT}/debug.cc ${CLD2_DIR_INT}/fixunicodevalue.cc ${CLD2_DIR_INT}/generated_entities.cc ${CLD2_DIR_INT}/generated_language.cc ${CLD2_DIR_INT}/generated_ulscript.cc ${CLD2_DIR_INT}/getonescriptspan.cc ${CLD2_DIR_INT}/lang_script.cc ${CLD2_DIR_INT}/offsetmap.cc ${CLD2_DIR_INT}/scoreonescriptspan.cc ${CLD2_DIR_INT}/tote.cc ${CLD2_DIR_INT}/utf8statetable.cc ${CLD2_DIR_INT}/cld_generated_cjk_uni_prop_80.cc ${CLD2_DIR_INT}/cld2_generated_cjk_compatible.cc ${CLD2_DIR_INT}/cld_generated_cjk_delta_bi_32.cc ${CLD2_DIR_INT}/generated_distinct_bi_0.cc ${CLD2_DIR_INT}/cld2_generated_quad0122.cc ${CLD2_DIR_INT}/cld2_generated_deltaocta0122.cc ${CLD2_DIR_INT}/cld2_generated_distinctocta0122.cc ${CLD2_DIR_INT}/cld_generated_score_quad_octa_0122.cc)
file(GLOB libcld2_full_glob ${libcld2_full_files})

add_executable(langsplit src/langsplit/langsplit.cpp)
target_link_libraries(langsplit ${libcld2_full_glob})


# build tests
if (BUILD_TEST)
    add_subdirectory(tests)
endif (BUILD_TEST)
