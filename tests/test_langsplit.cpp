
#include "gtest/gtest.h"
#include "../src/mono/filters/langsplitfilter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "boost/filesystem/operations.hpp"
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
  mono::filters::LangsplitFilter langsplitFilter = mono::filters::LangsplitFilter("../../tests/data_langsplit/", false);
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

void compare_stats(std::string input_file, std::string output_dir, std::string input_stat_file,
                   std::string expected_result_file) {
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
  boost::filesystem::create_directory(output_dir + "/stats");
  mono::filters::LangsplitFilter langsplitFilter = mono::filters::LangsplitFilter(output_dir, true);
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in(test1_input);
  boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
  out.push(langsplitFilter);
  out.push(boost::iostreams::null_sink());

  boost::iostreams::copy(in, out);

  // read langstats
  std::ifstream langstats_input(output_dir + "/" + input_stat_file, std::ios_base::in | std::ios_base::binary);
  if (!langstats_input.is_open()) {
    std::cerr << "langstat file not found!" << std::endl;
    FAIL();
  }
  std::stringstream langstats_output;
  boost::iostreams::filtering_streambuf<boost::iostreams::input> qin(langstats_input);
  boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;
  qout.push(boost::iostreams::gzip_decompressor());
  qout.push(langstats_output);

  boost::iostreams::copy(qin, qout);

  // compare outputs
  std::stringstream ss2;
  ss2 << test1_expected_output.rdbuf();
  ASSERT_EQ(langstats_output.str(), ss2.str());

  test1_input.close();
  test1_expected_output.close();
}


TEST(langsplit, test_simple) {
  std::string output_dir = "test_dir_langsplit";
  if (!boost::filesystem::create_directory(output_dir)) {
    std::cerr << "Output folder already exists! Please remove: " << output_dir << std::endl;
    FAIL();
  }

  compare("test1.in", "test1.out");
  compare_stats("test1.in", output_dir, "stats/langstats.0.gz", "test1_langstats.out");
  compare_stats("test1.in", output_dir, "stats/langstats.1.gz", "test1_langstats.out");

}
