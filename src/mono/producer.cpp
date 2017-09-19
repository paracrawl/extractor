
#include "readwarc.h"
#include "producer.h"
#include "../langsplit/langsplit.h"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"

#include <iostream>
#include <string>
#include <vector>


namespace mono {

    std::stringstream producer(std::string path) {
      std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
      if (!boost::filesystem::exists(path)) {
        std::cerr << "File not found!" << std::endl;
        return std::stringstream();
      }

      boost::iostreams::filtering_istream in;
      in.push(boost::iostreams::gzip_decompressor());
      in.push(file);

      ReadWARC reader;
      std::stringstream reader_ss = reader.parse<boost::iostreams::filtering_istream>(in);

      Langsplit langsplit;
      std::stringstream langsplit_ss = langsplit.process<std::stringstream>(reader_ss, std::vector<std::string>());

      return langsplit_ss;
    }
}
