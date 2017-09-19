
#include "gtest/gtest.h"
#include "../src/mono/readwarc.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void compare(std::string input_file, std::string expected_result_file) {
  mono::ReadWARC reader;

  std::ifstream test1_input(std::string("../../tests/data_readerwarc/") + input_file);
  std::ifstream test1_expected_output(std::string("../../tests/data_readerwarc/") + expected_result_file);

  if (!test1_input.is_open()) {
    std::cerr << "Input file not found!" << std::endl;
    FAIL();
  }

  if (!test1_expected_output.is_open()) {
    std::cerr << "Output file not found!" << std::endl;
    FAIL();
  }

  std::stringstream ss = reader.parse<std::ifstream>(test1_input);

  std::stringstream ss2;
  ss2 << test1_expected_output.rdbuf();
  ASSERT_EQ(ss.str(), ss2.str());

  test1_input.close();
  test1_expected_output.close();
}


TEST(readerwarc, test_simple) {
  compare("test1.in", "test1.out");
}

TEST(readerwarc, test_good_uri) {
  compare("test2.in", "test2.out");
}

TEST(readerwarc, test_bad_uri) {
  compare("test3.in", "test3.out");
};

TEST(readerwarc, test_newlines) {
  compare("test4.in", "test4.out");
};

TEST(readerwarc, test_header) {
  compare("test5.in", "test5.out");
};
