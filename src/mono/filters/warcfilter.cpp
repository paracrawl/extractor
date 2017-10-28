
#include "warcfilter.h"
#include "../../utils/common.h"

#include <iostream>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>


namespace mono {

    namespace filters {

        std::string WARCFilter::do_filter(const std::string &str_) {
          std::string str = str_;

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
                  state = 2;
                  return found_header;
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
              // PARSE AND OUTPUT CONTENT UNTIL HEADER IDENTIFIER FOUND]
              if (boost::starts_with(str, "WARC/1.0")) {
                state = 1;
              } else {
                if (!(str.empty())) {
                  utils::fix_utf8_string(str);
                  return str + '\n';
                }
              }
              break;
          }

          return "";
        };

        std::string WARCFilter::make_header(std::string const &uri) {
          std::ostringstream ss;
          ss << magic_number << " uri:" << boost::trim_left_copy(uri) << '\n';
          return ss.str();
        };

        std::string WARCFilter::parse_uri_header(std::string const &str) {
          std::string str_uri = str.substr(uri_guard.length());
          std::ostringstream ss;
          boost::regex expr{"^\\s*([^\\s]+)\\s*$"};
          boost::smatch match;

          if (boost::regex_match(str_uri, match, expr) && match.size() >= 1) {
            return make_header(match[0]);
          }

          return "";
        };

    }

}
