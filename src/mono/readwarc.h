
#ifndef EXTRACTOR_READWARC_H
#define EXTRACTOR_READWARC_H

#include "boost/algorithm/string/predicate.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/regex.hpp"


class ReadWARC {
public:

    template<typename T>
    std::stringstream parse(T &in) {
      std::stringstream ss;
      int state = 0;

      for (std::string str; std::getline(in, str);) {
        if (str.length() > 0 && str.back() == '\r')
          str.pop_back();

        switch (state) {

          case 0:
            // PARSE HEADER IDENTIFIER
            if (boost::starts_with(str, warc_guard))
              state = 1;
            break;

          case 1:
            // PARSE and OUTPUT URI HEADER
            if (boost::starts_with(str, uri_guard)) {
              std::string found_header = parse_uri_header(str);
              if (found_header.length() > 0) {
                ss << found_header;
                state = 2;
              } else {
                state = 0;
              }
            } else if (str.empty()) {
              state = 0;
            }
            break;

          case 2:
            // PARSE END-OF-HEADER NEW LINE
            if (str.empty())
              state = 3;
            break;

          case 3:
            // PARSE AND OUTPUT CONTENT UNTIL HEADER IDENTIFIER FOUND
            if (boost::starts_with(str, "WARC/1.0")) {
              state = 1;
            } else {
              if (!(str.empty()))
                ss << str << '\n';
            }
            break;
        }
      }

      return ss;
    }

private:

    const std::string warc_guard = "WARC/1.0";
    const std::string uri_guard = "WARC-Target-URI:";
    const std::string magic_number = "df6fa1abb58549287111ba8d776733e9";

    std::string make_header(std::string const &uri) {
      std::ostringstream ss;
      ss << magic_number << " uri:" << boost::trim_left_copy(uri) << '\n';
      return ss.str();
    }

    std::string parse_uri_header(std::string const &str) {
      std::string str_uri = str.substr(uri_guard.length());
      std::ostringstream ss;
      boost::regex expr{"^\\s*([^\\s]+)\\s*$"};
      boost::smatch match;

      if (boost::regex_match(str_uri, match, expr) && match.size() >= 1) {
        return make_header(match[0]);
      }

      return "";
    }


};

#endif //EXTRACTOR_READWARC_H
