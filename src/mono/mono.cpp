
#include "worker.h"
#include "../utils/logging.h"
#include "../utils/common.h"

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <thread>
#include <sstream>
#include <sys/resource.h>


namespace po = boost::program_options;

typedef utils::shared_vector<std::string> shared_vector_string;

namespace mono {

    long get_ulimit() {
      struct rlimit limit;

      if (getrlimit(RLIMIT_NOFILE, &limit) != 0) {
        return -1;
      }

      return (long) limit.rlim_cur;
    }

    void load_data_from_cin(shared_vector_string &data) {
      for (std::string path; std::getline(std::cin, path);) {
        path = boost::trim_copy(path);
        if (path.length() > 0)
          data.push(path);
      }
      data.reverse();  // so that pop_back returns in the original order
    }

    void start(int workers, bool curl, std::string output_folder, utils::compression_option input_compr,
               utils::compression_option output_compr) {

      shared_vector_string files_to_process;
      load_data_from_cin(files_to_process);
      LOG_INFO << files_to_process.size() << " files found to process.";

      utils::progress prog(files_to_process.size());
      boost::thread_group threads;
      for (int id = 0; id < workers; ++id) {
        std::string output_folder_thread = output_folder + "/" + std::to_string(id + 1);
        boost::filesystem::create_directory(output_folder_thread);
        threads.create_thread(
                boost::bind(run_worker, &files_to_process, &prog, curl, output_folder_thread, input_compr,
                            output_compr));
      }
      threads.join_all();
      prog.finish();
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
          ("workers", po::value<int>(), "set the number of workers")
          ("output", po::value<std::string>(), "set the output folder");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, reinterpret_cast<const char *const *>(argv), desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("workers")) {
    if (vm["workers"].as<int>() <= 0) {
      LOG_ERROR << "The number of workers has to be >= 1";
      throw 11;
    }

    LOG_INFO << "The number of workers set to "
             << vm["workers"].as<int>() << ".";
  }

  // check system resources
  long curr_ulimit = mono::get_ulimit();
  long sufficient_limit = 400 * vm["workers"].as<int>();
  LOG_INFO << "Current limit of open file descriptors: " << curr_ulimit;
  if (curr_ulimit == -1) {
    LOG_ERROR << "Failed to read available system resources!";
  } else if (curr_ulimit < sufficient_limit) {
    LOG_ERROR << "The limit of open file descriptors is too low - should be at least: " << sufficient_limit;
    throw 19;
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
    if (output_compr == utils::null) {
      LOG_ERROR << "Unsupported output compression option: " << vm["ocompression"].as<std::string>();
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

  if (vm.count("curl")) {
    LOG_INFO << "Using curl to download remote files. ";
  } else {
    LOG_INFO << "Local files will be processed. ";
  }


  mono::start(vm["workers"].as<int>(), vm.count("curl"), vm["output"].as<std::string>(), input_compr, output_compr);

  return 0;

}