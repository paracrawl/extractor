
#ifndef EXTRACTOR_LANGSPLIT_H
#define EXTRACTOR_LANGSPLIT_H

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>

#include "boost/algorithm/string/trim.hpp"
#include "../3rd_party/cld2/public/compact_lang_det.h"
#include "header.h"

using CLD2::int32;

typedef int32 Encoding;
static const Encoding UNKNOWN_ENCODING = 0;

class Langsplit {
public:

    template<typename T>
    std::stringstream process(T &input_stream, std::vector<std::string> modes) {

      int flags = get_flag(modes);
      bool print_stats = false;

      for (int i = 0; i < modes.size(); ++i) {
        if (modes.at(i) == "--printstats") {
          print_stats = true;
        }
      }

      std::stringstream ssout;
      std::ostringstream buffer;
      std::string line;
      std::string header;
      while (getline(input_stream, line)) {
        line = boost::trim_copy(line);
        if (line.empty()) {
          continue;
        }

        if (line.find(magic_number) == 0) {
          ssout << PrintLanguageStats(flags, header, buffer.str(), print_stats);

          buffer.clear();
          buffer.str(std::string(""));
          header = line;
        } else {
          buffer << line << "\n";
        }
      }

      ssout << PrintLanguageStats(flags, header, buffer.str(), print_stats);

      return ssout;
    }


private:

    const std::string magic_number = "df6fa1abb58549287111ba8d776733e9";

    int get_flag(std::vector<std::string> modes) {
      int flags = 0;
      for (int i = 0; i < modes.size(); ++i) {
        if (modes.at(i) == "--scoreasquads") {
          flags |= CLD2::kCLDFlagScoreAsQuads;
        } else if (modes.at(i) == "--html") {
          flags |= CLD2::kCLDFlagHtml;
        } else if (modes.at(i) == "--cr") {
          flags |= CLD2::kCLDFlagCr;
        } else if (modes.at(i) == "--verbose") {
          flags |= CLD2::kCLDFlagVerbose;
        } else if (modes.at(i) == "--echo") {
          flags |= CLD2::kCLDFlagEcho;
        } else if (modes.at(i) == "--besteffort") {
          flags |= CLD2::kCLDFlagBestEffort;
        }
      }

      return flags;
    }

    std::string PrintLanguageStats(const int flags, const std::string &header,
                                   const std::string &buffer, const bool print_stats) {
      std::stringstream ss;
      if (header.empty() || buffer.empty()) {
        return "";
      }

      const Header header_values(header);
      const std::string uri = header_values.get_uri();
      const std::string tld = header_values.get_tld();

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
        if (print_stats) {
          ss << output_stats(buffer, header, percent3, language3, normalized_score3);
        } else {
          for (int i = 0; i < static_cast<int>(resultchunkvector.size()); ++i) {
            const CLD2::ResultChunk &rc = resultchunkvector[i];
            CLD2::Language rc_lang = static_cast<CLD2::Language>(rc.lang1);

            if (rc_lang == CLD2::UNKNOWN_LANGUAGE) {
              continue;
            }

            const std::string lang_code = LanguageCode(rc_lang);
            ss << output_chunk(buffer, rc, header, lang_code);
          }
        }
      } else {
        std::cerr << "prediction unrealiable" << "\n";
      }

      return ss.str();
    }

    std::string output_chunk(const std::string buffer, const CLD2::ResultChunk &rc, const std::string header,
                             const std::string lang_code) {
      std::stringstream ss;
      const std::string chunk = std::string(buffer, rc.offset, rc.bytes);

      ss << header << " language:" << lang_code
         << " offset:" << rc.offset << " bytes:" << rc.bytes
         << "\n" << chunk << "\n";

      return ss.str();
    }

    std::string
    output_stats(const std::string buffer, const std::string header, int percent3[], CLD2::Language language3[],
                 double normalized_score3[]) {
      std::stringstream ss;

      ss << header << " bytes:" << buffer.size() << "\n";
      for (int i = 0; i < 3; i++) {
        if (percent3[i] > 0) {
          ss << LanguageName(language3[i]) << "\t" << percent3[i] << "\t"
             << normalized_score3[i] << "\n";
        }
      }

      return ss.str();
    }
};


#endif //EXTRACTOR_LANGSPLIT_H
