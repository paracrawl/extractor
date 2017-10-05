
#include "common.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <unordered_map>
#include <memory>


namespace utils {

    compression_option string_to_compression_option(std::string str) {
      if (str == "gzip") return gzip;
      if (str == "lzma") return lzma;
      if (str == "none") return none;

      return null;
    }

    std::string compression_option_to_string(compression_option compr) {
      if (compr == gzip) return "gzip";
      if (compr == lzma) return "lzma";
      if (compr == none) return "none";

      return "";
    }

    std::string get_compression_extension(compression_option compr) {
      if (compr == gzip) return "gz";
      if (compr == lzma) return "lz";
      if (compr == none) return "out";

      return "";
    }

    language_sink::language_sink(std::string output_folder_, utils::compression_option compr_) : output_folder(
            output_folder_), compr(compr_) {};

    void language_sink::output(std::string const &lang, std::string const &text) {
      std::unordered_map<std::string, std::shared_ptr<ostreambuf> >::iterator it;

      it = sinkmap.find(lang);
      if (it == sinkmap.end()) {
        add_language_sink(lang);
      }

      std::ostream outf(sinkmap.at(lang).get());
      outf.write(text.c_str(), text.size());
    }

    void language_sink::add_language_sink(std::string lang) {
      std::string ofilesink_path = get_langfile_path(output_folder, lang).string();

      auto out = std::make_shared<ostreambuf>();
      sinkmap.insert(std::make_pair(lang, out));

      std::ios_base::openmode flags = std::ofstream::app;

      if (compr == utils::gzip) {
        sinkmap.at(lang)->push(boost::iostreams::gzip_compressor());
        flags |= std::ofstream::binary;
      }

      sinkmap.at(lang)->push(boost::iostreams::file_sink(ofilesink_path, flags));
    }

    boost::filesystem::path language_sink::get_langfile_path(std::string folder, std::string lang) {
      boost::format path = boost::format("%s/text.%s.%s") % folder % lang % get_compression_extension(compr);
      return boost::filesystem::path(path.str());
    }

    progress::progress(int total_) : current_progress(0), total(total_) {
      std::cout << std::endl;
      show_bar();
    }

    void progress::increment() {
      boost::lock_guard<boost::mutex> lock(mutex);
      ++current_progress;
      show_bar();
    }

    void progress::show_bar() {
      int bar_width = 70;

      std::cout << " " << current_progress << "/" << total << " [";
      int pos = bar_width * current_progress / float(total);
      for (int i = 0; i < bar_width; ++i) {
        if (i < pos)
          std::cout << "=";
        else if (i == pos)
          std::cout << ">";
        else
          std::cout << " ";
      }
      std::cout << "] " << int(current_progress / float(total) * 100.0) << " %\r";
      std::cout.flush();
    };

    void progress::finish() {
      std::cout << std::endl;
      std::cout << "Done." << std::endl;
    }
}