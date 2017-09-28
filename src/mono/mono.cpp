
#include "pcqueue.h"
#include "producer.h"
#include "../utils/logging.h"
#include "../utils/common.h"

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <thread>
#include <sstream>


namespace po = boost::program_options;

typedef utils::shared_vector<std::string> shared_vector_string;

namespace mono {

    void thread_producer(util::PCQueue<int> *pcq) {
      int r = rand();
      (*pcq).Produce(r);
      std::cout << "Producing: " << r << std::endl;
    }

    void thread_consumer(util::PCQueue<int> *pcq) {
      std::chrono::milliseconds sleepDuration(100);
      std::this_thread::sleep_for(sleepDuration);

      int c = (*pcq).Consume();
      std::cout << "Consumed: " << c << std::endl;
    }

    void load_data_from_cin(shared_vector_string &data) {
      for (std::string path; std::getline(std::cin, path);) {
        data.push(boost::trim_copy(path));
      }
      data.reverse();  // so that pop_back returns in the original order
    }

    int start(bool curl, std::string output_folder, utils::compression_option input_compr, utils::compression_option output_compr) {
      shared_vector_string files_to_process;
      load_data_from_cin(files_to_process);
      LOG_INFO << files_to_process.size() << " files found to process.";

      while (files_to_process.size() > 0) {
        std::string path = files_to_process.pop();
        if (curl) {
          producer_curl(path, output_folder, input_compr, output_compr);
        } else {
          producer_file(path, output_folder, input_compr, output_compr);
        }
      }

      return 0;
    }

}

int main(int argc, char *argv[]) {
  logging::init();

  po::options_description desc("Allowed options");
  desc.add_options()
          ("help", "produce help message")
          ("curl", "set the number of producers")
          ("icompression", po::value<std::string>(), "set expected input compression")
          ("ocompression", po::value<std::string>(), "set output compression")
          ("producers", po::value<int>(), "set the number of producers")
          ("consumers", po::value<int>(), "set the number of consumers")
          ("output", po::value<std::string>(), "set the output folder");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, reinterpret_cast<const char *const *>(argv), desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("producers")) {
    LOG_INFO << "The number of producers set to "
             << vm["producers"].as<int>() << ".";
  }

  if (vm.count("consumers")) {
    LOG_INFO << "The number of consumers set to "
             << vm["consumers"].as<int>() << ".";
  }

  utils::compression_option input_compr;
  utils::compression_option output_compr;

  if (vm.count("icompression")) {
    input_compr = utils::string_to_compression_option(vm["icompression"].as<std::string>());
    if (input_compr == utils::null || input_compr == utils::lzma) {
      LOG_ERROR << "Unsupported input compression option: " << utils::compression_option_to_string(input_compr);
      throw 11;
    }
    LOG_INFO << "Expecting input compression: " << utils::compression_option_to_string(input_compr);
  } else {
    input_compr = utils::none;
    LOG_INFO << "Expecting uncompressed input files. ";
  }

  if (vm.count("ocompression")) {
    output_compr = utils::string_to_compression_option(vm["ocompression"].as<std::string>());
    if (output_compr == utils::null || output_compr == utils::lzma) {
      LOG_ERROR << "Unsupported output compression option: " << utils::compression_option_to_string(output_compr);
      throw 11;
    }
    LOG_INFO << "Setting output compression: " << utils::compression_option_to_string(output_compr);
  } else {
    output_compr = utils::none;
    LOG_INFO << "Using no compression for output files. ";
  }

  if (vm.count("output")) {
    boost::filesystem::path output_dir(vm["output"].as<std::string>());
    if (boost::filesystem::create_directory(output_dir)) {
      LOG_INFO << "Outputting to: " << output_dir.string();
    } else {
      LOG_ERROR << "The output folder already exists!\n";
      throw 17;
    }
  } else {
    LOG_ERROR << "The output folder was not set!\n";
    throw 10;
  }


  return mono::start(vm.count("curl"), vm["output"].as<std::string>(), input_compr, output_compr);

}