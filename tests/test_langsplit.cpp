
#include "gtest/gtest.h"
#include "../src/langsplit/langsplitfilter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>


void compare(std::string input_file, std::string expected_result_file) {
  // load files
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

  // apply filter
  std::stringstream output;
  mono::LangsplitFilter langsplitFilter = mono::LangsplitFilter();
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in(test1_input);
  boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
  out.push(langsplitFilter);
  out.push(output);

  boost::iostreams::copy(in, out);

  // compare outputs
  std::stringstream ss2;
  ss2 << test1_expected_output.rdbuf();
  ASSERT_EQ(output.str(), ss2.str());

  test1_input.close();
  test1_expected_output.close();
}


TEST(langsplit, test_simple) {
  compare("test1.in", "test1.out");
}
