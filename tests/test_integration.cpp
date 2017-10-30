
#include "gtest/gtest.h"
#include "../src/mono/worker.h"
#include "../src/utils/common.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


typedef std::vector<std::pair<std::string, std::string>> pair_files_vec;


void compare_lengths(std::string output_file, std::string expected_file) {
  std::cout << "Comparing: " << output_file << " and " << expected_file << std::endl;

  std::ifstream ja_output(output_file);
  std::ifstream ja_expected_output("../../tests/data_integration/" + expected_file);

  if (!ja_output.is_open()) {
    std::cerr << "Output file not found!" << std::endl;
    FAIL();
  }

  if (!ja_expected_output.is_open()) {
    std::cerr << "Expected file not found!" << std::endl;
    FAIL();
  }

  std::stringstream ss1;
  ss1 << ja_output.rdbuf();
  ja_output.close();

  std::stringstream ss2;
  ss2 << ja_expected_output.rdbuf();
  ja_expected_output.close();

  ASSERT_EQ(ss1.str().length(), ss2.str().length());
}

void compare(std::string test_name, std::string input_file, std::string output_folder, pair_files_vec test_files,
             utils::compression_option in_compr, utils::compression_option out_compr) {

  std::string test1_input_path = std::string("../../tests/data_integration/") + input_file;

  if (!boost::filesystem::exists(test1_input_path)) {
    std::cerr << "Input file not found!" << std::endl;
    FAIL();
  }

  boost::filesystem::path output_dir(output_folder);

  if (boost::filesystem::create_directory(boost::filesystem::path(output_dir.string() + "/" + test_name))) {

    mono::worker_file(test1_input_path, false, output_folder + "/" + test_name, in_compr, out_compr);
    for (auto a: test_files) {
      compare_lengths(output_folder + "/" + test_name + "/" + a.first, a.second);
    }

  } else {
    std::cerr << "Output folder already exists! Please remove: " << output_folder << std::endl;
    FAIL();
  }

}


TEST(integration, test_simple_gzip_to_langsplit) {
  std::string output_dir = "test_dir_integration";
  if (!boost::filesystem::create_directory(output_dir)) {
    std::cerr << "Output folder already exists! Please remove: " << output_dir << std::endl;
    FAIL();
  }

  pair_files_vec test1_files;
  test1_files.push_back(std::make_pair("text.ja.out", "test1_ja.out"));
  test1_files.push_back(std::make_pair("text.en.out", "test1_en.out"));

  pair_files_vec test2_files;
  test2_files.push_back(std::make_pair("text.de.out", "test2_de.out"));
  test2_files.push_back(std::make_pair("text.ru.out", "test2_ru.out"));

  pair_files_vec test3_files;
  test3_files.push_back(std::make_pair("text.en.out", "test3_en.out"));

  compare("test1", "test1.in.gz", output_dir, test1_files, utils::gzip, utils::none);
  compare("test2", "test2.in.gz", output_dir, test2_files, utils::gzip, utils::none);
  compare("test3", "test3.in", output_dir, test3_files, utils::none, utils::none);
}