
#ifndef EXTRACTOR_LANGSPLIT_H
#define EXTRACTOR_LANGSPLIT_H

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>

#include "../3rd_party/cld2/public/compact_lang_det.h"
#include "header.h"

using CLD2::int32;

typedef int32 Encoding;
static const Encoding UNKNOWN_ENCODING = 0;
static const char *magic_number = "df6fa1abb58549287111ba8d776733e9";

class Langsplit {
public:

    template<typename T>
    std::stringstream process(T &input_stream) {
      std::vector<std::string> argv = {"--printchunks"};

      int flags = 0;
      bool print_stats = false;
      bool print_chunks = false;
      for (int i = 0; i < argv.size(); ++i) {
        if (argv.at(i) == "--scoreasquads") {
          flags |= CLD2::kCLDFlagScoreAsQuads;
        }
        if (argv.at(i) == "--html") {
          flags |= CLD2::kCLDFlagHtml;
        }
        if (argv.at(i) == "--cr") {
          flags |= CLD2::kCLDFlagCr;
        }
        if (argv.at(i) == "--verbose") {
          flags |= CLD2::kCLDFlagVerbose;
        }
        if (argv.at(i) == "--echo") {
          flags |= CLD2::kCLDFlagEcho;
        }
        if (argv.at(i) == "--besteffort") {
          flags |= CLD2::kCLDFlagBestEffort;
        }
        if (argv.at(i) == "--printstats") {
          print_stats = true;
        }
        if (argv.at(i) == "--printchunks") {
          print_chunks = true;
        }
      }

      std::stringstream ssout;
      std::ostringstream buffer;
      string line;
      string header;
      while (getline(input_stream, line)) {
        line = trim(line);
        if (line.empty()) {
          continue;
        }
        if (line.find(magic_number) == 0) {
          if (print_stats || print_chunks) {
            ssout << PrintLanguageStats(flags, header, buffer.str(), print_chunks);
          }
          buffer.clear();
          buffer.str(string(""));
          header = line;
        } else {
          buffer << line << std::endl;
        }
      }

      if (print_stats || print_chunks) {
        ssout << PrintLanguageStats(flags, header, buffer.str(), print_chunks);
      }

      return ssout;
    }

    std::string PrintLanguageStats(const int flags, const string &header,
                                   const string &buffer, const bool print_chunks) {
      std::stringstream ss;
      if (header.empty() || buffer.empty()) {
        return "";
      }

      const Header header_values(header);
      const string uri = header_values.get_uri();
      const string tld = header_values.get_tld();

      bool is_plain_text = true;
      CLD2::CLDHints cld_hints = {NULL, NULL, UNKNOWN_ENCODING,
                                  CLD2::UNKNOWN_LANGUAGE};
      if (!tld.empty()) {
        cld_hints.tld_hint = tld.c_str();
      }
      CLD2::Language language3[3];
      int percent3[3];
      double normalized_score3[3];
      int valid_prefix_bytes;

      CLD2::ResultChunkVector resultchunkvector;
      int text_bytes;
      bool is_reliable;

      CLD2::ExtDetectLanguageSummaryCheckUTF8(
              buffer.c_str(), buffer.size(), is_plain_text, &cld_hints, flags,
              language3, percent3, normalized_score3, &resultchunkvector, &text_bytes,
              &is_reliable, &valid_prefix_bytes);

      if (is_reliable) {
        if (print_chunks) {
          for (int i = 0; i < static_cast<int>(resultchunkvector.size()); ++i) {
            const CLD2::ResultChunk &rc = resultchunkvector[i];
            CLD2::Language rc_lang = static_cast<CLD2::Language>(rc.lang1);
            if (rc_lang == CLD2::UNKNOWN_LANGUAGE) {
              continue;
            }
            const char *lang_code = LanguageCode(rc_lang);
            const string chunk = string(buffer, rc.offset, rc.bytes);

            ss << header << " language:" << lang_code
               << " offset:" << rc.offset << " bytes:" << rc.bytes
               << std::endl;
            ss << chunk << std::endl;
          }
        } else {
          // only print some statistics
          ss << header << " bytes:" << buffer.size() << std::endl;
          for (int i = 0; i < 3; i++) {
            if (percent3[i] > 0) {
              const char *lang_name = LanguageName(language3[i]);
              ss << lang_name << "\t" << percent3[i] << "\t"
                 << normalized_score3[i] << std::endl;
            }
          }
        }
      } else {
        std::cerr << "prediction unrealiable" << std::endl;
      }

      return ss.str();
    }

private:
    static string trim(const string &s) {
      auto wsfront = std::find_if_not(s.begin(), s.end(),
                                      [](int c) { return std::isspace(c); });
      auto wsback = std::find_if_not(s.rbegin(), s.rend(),
                                     [](int c) { return std::isspace(c); }).base();
      return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }
};


#endif //EXTRACTOR_LANGSPLIT_H
