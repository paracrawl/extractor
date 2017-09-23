
#include "langsplitfilter.h"
#include "header.h"
#include "../3rd_party/cld2/public/compact_lang_det.h"
#include "boost/algorithm/string/trim.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>


namespace mono {

    std::string LangsplitFilter::do_filter(const std::string &str_) {
      std::string line = boost::trim_copy(str_);

      if (line.empty()) {
        return "";
      }

      if (line.find(magic_number) == 0) {
        std::string res = PrintLanguageStats(flags, header, text_buffer, print_stats);

        text_buffer.erase();
        header = line;

        return res;
      } else {
        text_buffer += line + "\n";
      }

      return "";
    }

    int LangsplitFilter::get_flag(std::vector<std::string> modes) {
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

    std::string LangsplitFilter::PrintLanguageStats(const int flags, const std::string &header,
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

    std::string
    LangsplitFilter::output_chunk(const std::string buffer, const CLD2::ResultChunk &rc, const std::string header,
                                  const std::string lang_code) {
      std::stringstream ss;
      const std::string chunk = std::string(buffer, rc.offset, rc.bytes);

      ss << header << " language:" << lang_code
         << " offset:" << rc.offset << " bytes:" << rc.bytes
         << "\n" << chunk << "\n";

      return ss.str();
    }

    std::string
    LangsplitFilter::output_stats(const std::string buffer, const std::string header, int percent3[],
                                  CLD2::Language language3[],
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

}