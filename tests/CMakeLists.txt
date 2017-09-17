
file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(../)

# gtest
enable_testing()
find_package(GTest REQUIRED)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif ()


foreach (source ${srcs})
    get_filename_component(tname ${source} NAME_WE)
    message("TEST name: " "${tname}")

    # target
    add_executable(${tname} ${source})
    target_link_libraries(${tname} ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})

    # Install
    install(TARGETS ${tname} DESTINATION tests)
endforeach (source)
