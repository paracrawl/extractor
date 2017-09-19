
#include "readwarc.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"

#include <iostream>


int main(int argc, char *argv[]) {
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
  in.push(boost::iostreams::gzip_decompressor());
  in.push(file);

  mono::ReadWARC reader;
  std::stringstream ss = reader.parse<boost::iostreams::filtering_istream>(in);
  std::cout << ss.str();

}