
#ifndef EXTRACTOR_MONO_FILTERS_STRING_UTIL_H
#define EXTRACTOR_MONO_FILTERS_STRING_UTIL_H

#include "boost/algorithm/string/trim.hpp"

#include <sstream>
#include <algorithm>
#include <string>
#include <vector>


namespace mono {

    namespace filters {

        class StringUtil {
        public:
            static bool EndsWith(std::string const &s, const std::string &end) {
              return (s.length() >= end.length() && s.compare(s.length() - end.length(), end.length(), end) == 0);
            }

            static std::string ToLower(const std::string &s) {
              std::string lower(s);
              std::transform(s.begin(), s.end(), lower.begin(), ::tolower);
              return lower;
            }

            static std::vector<std::string> Split(const std::string &s, const char delim = ' ') {
              std::vector<std::string> tokens;
              std::stringstream ss(s);
              std::string token;
              while (std::getline(ss, token, delim)) {
                if (!token.empty()) {
                  tokens.push_back(token);
                }
              }
              return tokens;
            }

            static void TrimRepeatedSpace(std::string *s) {
              s->erase(std::unique(s->begin(), s->end(),
                                   [](char a, char b) { return std::isspace(a) && std::isspace(b); }),
                       s->end());
            }

            static std::string TrimRepeatedWhitespace(const std::string &s) {
              std::stringstream ss(s);
              std::ostringstream oss;
              std::string line;
              while (std::getline(ss, line)) {
                line = boost::trim_copy(line);
                if (!line.empty()) {
                  TrimRepeatedSpace(&line);
                  oss << line << "\n";
                }
              }
              return oss.str();
            }
        };

    }

}


#endif //EXTRACTOR_MONO_FILTERS_STRING_UTIL_H
