
#include "gtest/gtest.h"
#include "../src/mono/producer.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void compare_lengths(std::string input_file, std::string expected_result_file) {

  std::string test1_input_path = std::string("../../tests/data_integration/") + input_file;
  std::ifstream test1_expected_output(std::string("../../tests/data_integration/") + expected_result_file);

  if (!boost::filesystem::exists(test1_input_path)) {
    std::cerr << "Input file not found!" << std::endl;
    FAIL();
  }

  if (!test1_expected_output.is_open()) {
    std::cerr << "Output file not found!" << std::endl;
    FAIL();
  }

  std::stringstream ss = mono::producer_file(test1_input_path);

  std::stringstream ss2;
  ss2 << test1_expected_output.rdbuf();
  test1_expected_output.close();

  ASSERT_EQ(ss.str().length(), ss2.str().length());

}


TEST(integration, test_simple_gzip_to_langsplit) {
  compare_lengths("test1.in.gz", "test1.out");
}