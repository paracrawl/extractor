
# gtest
enable_testing()
find_package(GTest REQUIRED)
if (GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
endif ()

# test_readerwarc
add_executable(test_readerwarc test_readerwarc.cpp ../src/mono/filters/warcfilter.cpp ../src/utils/common.cpp)
target_link_libraries(test_readerwarc ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})
install(TARGETS test_readerwarc DESTINATION tests)

# test_langsplit
add_executable(test_langsplit test_langsplit.cpp ../src/mono/filters/langsplitfilter.cpp ../src/mono/buffered_map.cpp ../src/utils/common.cpp ../src/utils/logging.cpp)
target_link_libraries(test_langsplit ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} cld2_lib)
install(TARGETS test_langsplit DESTINATION tests)

# test_integration
add_executable(test_integration test_integration.cpp ../src/mono/filters/langcollectorfilter.cpp ../src/mono/filters/langsplitfilter.cpp ../src/mono/filters/warcfilter.cpp ../src/mono/language_sink.cpp ../src/mono/buffered_map.cpp ../src/mono/worker.cpp ../src/utils/common.cpp ../src/utils/compression.cpp ../src/utils/curldownloader.cpp ../src/utils/logging.cpp)
target_link_libraries(test_integration ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} cld2_lib)
install(TARGETS test_integration DESTINATION tests)

# test utils
add_executable(test_utils test_utils.cpp ../src/utils/common.cpp)
target_link_libraries(test_utils ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
install(TARGETS test_utils DESTINATION tests)
