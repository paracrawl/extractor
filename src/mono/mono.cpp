
#include <iostream>
#include <fstream>
#include <thread>

#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"

#include "pcqueue.h"
#include "readwarc.h"
#include "../langsplit/langsplit.h"


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

void producer(std::string path) {
  std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
  if (!boost::filesystem::exists(path)) {
    std::cerr << "File not found!" << std::endl;
    return;
  }

  boost::iostreams::filtering_istream in;
  in.push(boost::iostreams::gzip_decompressor());
  in.push(file);

  ReadWARC reader;
  std::stringstream reader_ss = reader.parse<boost::iostreams::filtering_istream>(in);

  Langsplit langsplit;
  std::vector<std::string> modes = {};
  std::stringstream langsplit_ss = langsplit.process<std::stringstream>(reader_ss, modes);

  std::cout << langsplit_ss.str();
}

int main() {
  util::PCQueue<int> pcq(3);

  for (std::string path; std::getline(std::cin, path);) {
    producer(path);
  }

  return 0;
}