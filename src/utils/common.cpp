
#include "common.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <map>


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

    language_sink::language_sink(std::string output_folder_, utils::compression_option compr_) : output_folder(output_folder_), compr(compr_) {};

    void language_sink::output(std::string const &lang, std::string const &text) {
      std::map<std::string, std::pair<ostreambuf *, ofilesink *>>::iterator it;

      it = sinkmap.find(lang);
      if (it == sinkmap.end()) {
        add_language_sink(lang);
      }

      std::pair<ostreambuf *, ofilesink *> q = sinkmap.at(lang);
      ostreambuf *p = q.first;
      std::ostream outf(p);
      outf.write(text.c_str(), text.size());
    }

    void language_sink::close_all() {
      for (auto &pair : sinkmap) {
        delete pair.second.first;
        delete pair.second.second;
      }
    }

    void language_sink::add_language_sink(std::string lang) {
      std::string ofilesink_path = get_langfile_path(output_folder, lang).string();
      sinkmap.insert(std::make_pair(lang, std::make_pair(
              new ostreambuf(),
              new ofilesink(ofilesink_path))));

      if (compr == utils::gzip) {
        sinkmap.at(lang).first->push(boost::iostreams::gzip_compressor());
      }

      sinkmap.at(lang).first->push(*sinkmap.at(lang).second);
    }

    boost::filesystem::path language_sink::get_langfile_path(std::string folder, std::string lang) {
      boost::format path = boost::format("%s/text.%s.%s") % folder % lang % get_compression_extension(compr);
      return boost::filesystem::path(path.str());
    }

}