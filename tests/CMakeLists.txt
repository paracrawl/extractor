
file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# gtest
enable_testing()
find_package(GTest REQUIRED)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif ()

# test_readerwarc
add_executable(test_readerwarc test_readerwarc.cpp)
target_link_libraries(test_readerwarc ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS test_readerwarc DESTINATION tests)

# test_langsplit
add_executable(test_langsplit test_langsplit.cpp)
target_link_libraries(test_langsplit ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${libcld2_full_glob})
install(TARGETS test_langsplit DESTINATION tests)
