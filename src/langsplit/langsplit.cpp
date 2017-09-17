
#include "langsplit.h"

#include <iostream>
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/iostreams/filtering_stream.hpp"


int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Wrong parameters!" << std::endl;
    return 1;
  }

  if (!boost::filesystem::exists(argv[1])) {
    std::cerr << "File not found!" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1], std::ios_base::in | std::ios_base::binary);

  boost::iostreams::filtering_istream in;
  in.push(file);


  Langsplit langsplit;
  std::stringstream ss = langsplit.process<boost::iostreams::filtering_istream>(in);
  std::cout << ss.str();

  return 0;
}