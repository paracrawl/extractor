
#include "gtest/gtest.h"

#include <fstream>

#include "../src/langsplit/langsplit.h"


void compare(std::string input_file, std::string expected_result_file) {
  Langsplit langsplit;

  std::ifstream test1_input(std::string("../../tests/data_langsplit/") + input_file);
  std::ifstream test1_expected_output(std::string("../../tests/data_langsplit/") + expected_result_file);

  if (!test1_input.is_open()) {
    std::cerr << "Input file not found!" << std::endl;
    FAIL();
  }

  if (!test1_expected_output.is_open()) {
    std::cerr << "Output file not found!" << std::endl;
    FAIL();
  }

  std::stringstream ss = langsplit.process<std::ifstream>(test1_input);

  std::stringstream ss2;
  ss2 << test1_expected_output.rdbuf();
  ASSERT_EQ(ss.str(), ss2.str());

  test1_input.close();
  test1_expected_output.close();
}


TEST(readerwarc, test_simple) {
  compare("test1.in", "test1.out");
}
