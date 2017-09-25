
#define BOOST_LOG_DYN_LINK 1

#include "pcqueue.h"
#include "producer.h"
#include "../utils/logging.h"
#include "../utils/common.h"

#include <boost/program_options.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <thread>
#include <sstream>


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
}


namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  logging::init();

  po::options_description desc("Allowed options");
  desc.add_options()
          ("help", "produce help message")
          ("curl", "set the number of producers")
          ("compression", po::value<std::string>(), "set expected compression")
          ("producers", po::value<int>(), "set the number of producers")
          ("consumers", po::value<int>(), "set the number of consumers");

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
  } else {
    std::cerr << "The number of producers was not set.\n";
    throw 10;
  }

  if (vm.count("consumers")) {
    LOG_INFO << "The number of consumers set to "
             << vm["consumers"].as<int>() << ".";
  } else {
    std::cerr << "The number of consumers was not set.\n";
    throw 10;
  }

  utils::compression_option compr;

  if (vm.count("compression")) {
    compr = utils::string_to_compression_option(vm["compression"].as<std::string>());
    if (compr == utils::null) {
      LOG_ERROR << "Unrecognised compression option!";
      throw 11;
    }
    LOG_INFO << "Expecting compression: " << vm["compression"].as<std::string>();
  } else {
    compr = utils::none;
    LOG_INFO << "Expecting uncompressed files. ";
  }

  if (vm.count("curl")) {
    LOG_INFO << "Using curl to download remote files. ";
    std::stringstream ss = mono::producer_curl(argv[1], compr);

  } else {
    LOG_INFO << "Local files will be processed. ";
    std::stringstream ss = mono::producer_file(argv[1], compr);
  }


  return 0;
}