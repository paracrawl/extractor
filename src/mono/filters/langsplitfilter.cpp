
#include "langsplitfilter.h"
#include "../buffered_map.h"
#include "../../utils/common.h"
#include "../../utils/logging.h"
#include "compact_lang_det.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <compact_lang_det.h>


namespace mono {

    namespace filters {

        LangsplitFilter::LangsplitFilter(std::string output_folder_, bool print_stats_) :
                boost::iostreams::line_filter(true),
                print_stats(print_stats_),
                bmap(output_folder_, 1000000),
                output_folder(output_folder_),
                header(""),
                text_buffer(""),
                num_reliable(0),
                num_unreliable(0) {
          flags = get_flag(modes);
        }

        LangsplitFilter::~LangsplitFilter() {
          if (num_reliable == 0 && num_unreliable == 0) {
            return;
          }

          logging::log_reliable(output_folder, num_reliable, num_unreliable);
        }

        std::string LangsplitFilter::do_filter(const std::string &str_) {
          std::string line = boost::trim_copy(str_);

          if (line.empty()) {
            return "";
          }

          if (boost::starts_with(line, magic_number)) {
            std::string res = PrintLanguageStats(flags, header, text_buffer);

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
          for (int i = 0; i < (int) modes.size(); ++i) {
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
                                                        const std::string &buffer) {
          std::stringstream ss;
          if (header.empty() || buffer.empty()) {
            return "";
          }

          const Header header_values(header);
          const std::string uri = header_values.get_uri();
          utils::parse_uri parsed_uri(uri);

          CLD2::CLDHints cld_hints = {NULL, NULL, UNKNOWN_ENCODING,
                                      CLD2::UNKNOWN_LANGUAGE};
          if (!parsed_uri.get_tld().empty()) {
            cld_hints.tld_hint = parsed_uri.get_tld().c_str();
          }

          CLD2::Language language3[3];
          int percent3[3];
          double normalized_score3[3];
          int valid_prefix_bytes;

          CLD2::ResultChunkVector resultchunkvector;
          int text_bytes;
          bool is_reliable;

          CLD2::ExtDetectLanguageSummaryCheckUTF8(
                  buffer.c_str(), buffer.size(), true, &cld_hints, flags,
                  language3, percent3, normalized_score3, &resultchunkvector, &text_bytes,
                  &is_reliable, &valid_prefix_bytes);

          if (is_reliable) {
            ++num_reliable;
            for (int i = 0; i < static_cast<int>(resultchunkvector.size()); ++i) {
              const CLD2::ResultChunk &rc = resultchunkvector[i];
              CLD2::Language rc_lang = static_cast<CLD2::Language>(rc.lang1);

              if (rc_lang == CLD2::UNKNOWN_LANGUAGE) {
                continue;
              }

              const std::string lang_code = LanguageCode(rc_lang);
              ss << output_chunk(buffer, rc, header, lang_code);

              if (print_stats) {
                bmap.add(std::make_pair(parsed_uri.get_domain(), lang_code), static_cast<long>(rc.bytes));
              }
            }

          } else {
            ++num_unreliable;
          }

          return ss.str();
        }

        std::string
        LangsplitFilter::output_chunk(const std::string buffer, const CLD2::ResultChunk &rc, const std::string header,
                                      const std::string lang_code) {
          std::stringstream ss;
          const std::string chunk = std::string(buffer, rc.offset, rc.bytes);

          ss << header << " language:" << lang_code << " bytes:" << rc.bytes << "\n" << chunk << "\n";
          return ss.str();
        }

    }

}