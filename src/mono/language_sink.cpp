

#include "language_sink.h"
#include "../utils/compression.h"
#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <unordered_map>
#include <memory>


namespace mono {

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
      // add new stream to language map
      auto out = std::make_shared<ostreambuf>();
      sinkmap.insert(std::make_pair(lang, out));

      // flags
      std::ios_base::openmode flags = std::ofstream::app;
      if (compr == utils::gzip || compr == utils::gzip || compr == utils::gzip || compr == utils::gzip) {
        flags |= std::ofstream::binary;
      }

      // add file sink with compression
      utils::add_compression(sinkmap.at(lang), compr);
      std::string ofilesink_path = get_langfile_path(output_folder, lang).string();
      sinkmap.at(lang)->push(boost::iostreams::file_sink(ofilesink_path, flags));
    }

    boost::filesystem::path language_sink::get_langfile_path(std::string folder, std::string lang) {
      boost::format path = boost::format("%s/text.%s.%s") % folder % lang % get_compression_extension(compr);
      return boost::filesystem::path(path.str());
    }

}