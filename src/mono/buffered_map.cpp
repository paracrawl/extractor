
#include "buffered_map.h"
#include "../utils/compression.h"

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/filesystem.hpp>


#include <iostream>
#include <string>


namespace mono {

    buffered_map::buffered_map(std::string output_folder_, unsigned long buffer_size_) : output_folder(output_folder_),
                                                                                         buffer_size(buffer_size_),
                                                                                         file_no(0) {};

    void buffered_map::add(std::pair<std::string, std::string> key, long value) {
      if (domain_map.size() + 1 > buffer_size) {
        flush();
        domain_map.clear();
      }

      std::map<std::pair<std::string, std::string>, long>::iterator it = domain_map.find(key);
      if (it != domain_map.end()) {
        it->second += value;
      } else {
        domain_map.insert(std::pair<std::pair<std::string, std::string>, long>(key, value));
      }

    }

    void buffered_map::flush() {
      // find an unused filename
      std::string candidate_ouput_path = output_folder + "/stats/langstats." + std::to_string(file_no) + ".gz";
      while (boost::filesystem::exists(candidate_ouput_path)) {
        ++file_no;
        candidate_ouput_path = output_folder + "/stats/langstats." + std::to_string(file_no) + ".gz";
      }

      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;
      qout.push(boost::iostreams::gzip_compressor());
      qout.push(boost::iostreams::file_sink(candidate_ouput_path, std::ofstream::out | std::ofstream::binary));

      // output domain_map into a file
      std::ostream outf(&qout);
      std::map<std::pair<std::string, std::string>, long>::iterator it = domain_map.begin();
      std::string sep = "\t";
      std::string nl = "\n";
      for (; it != domain_map.end(); ++it) {
        outf.write(it->first.first.c_str(), it->first.first.size());
        outf.write(sep.c_str(), sep.size());
        outf.write(it->first.second.c_str(), it->first.second.size());
        outf.write(sep.c_str(), sep.size());
        outf.write(std::to_string(it->second).c_str(), std::to_string(it->second).size());
        outf.write(nl.c_str(), nl.size());
      }

      // close and increment file number
      ++file_no;
    }

    size_t buffered_map::size() {
      return domain_map.size();
    }


}