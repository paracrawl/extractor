
#include "langcollector.h"
#include "../utils/common.h"

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <map>
#include <fstream>


namespace mono {

    LangCollectorFilter::LangCollectorFilter(std::string output_folder_, utils::compression_option compr_)
            : boost::iostreams::line_filter(true),
              header(""), text_buffer(""),
              ls(output_folder_, compr_) {};

    std::string LangCollectorFilter::do_filter(const std::string &str_) {
      std::string line = boost::trim_copy(str_);

      if (line.empty()) {
        return "";
      }

      if (boost::starts_with(line, magic_number)) {
        if (header.size() > 0) {
          std::string res = header + '\n' + text_buffer;
          std::string lang = parse_language(header);

          text_buffer.erase();

          if (lang.length() == 0) {
            header = "";
            return "";
          }

          output_to_langsink(lang, res);

        }

        // load header
        header = line;

        return "";

      } else {
        text_buffer += line + "\n";
      }

      return "";
    }

    std::string LangCollectorFilter::parse_language(std::string const &str) {
      std::string found = find_language(str);

      if (found.length() > 0) {
        return boost::trim_copy(found.substr(language_guard.length()));
      }

      return "";

    };

    std::string LangCollectorFilter::find_language(std::string const &str) {
      std::string str_uri = str.substr(magic_number.length());
      std::ostringstream ss;
      boost::regex expr{language_guard + "\\s*[a-zA-Z0-9]+\\s*"};
      boost::smatch match;

      if (boost::regex_search(str_uri, match, expr) && match.size() >= 1) {
        return match[0];
      }

      return "";
    };

    void LangCollectorFilter::output_to_langsink(std::string const &lang, std::string const &text) {
      ls.output(lang, text);
    }

}