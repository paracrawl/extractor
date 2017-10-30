
#ifndef EXTRACTOR_MONO_FILTERS_LANGSPLITFILTER_H
#define EXTRACTOR_MONO_FILTERS_LANGSPLITFILTER_H

#include "header.h"
#include "compact_lang_det.h"
#include "../buffered_map.h"

#include <boost/iostreams/filter/line.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


typedef CLD2::int32 Encoding;
static const Encoding UNKNOWN_ENCODING = 0;

namespace mono {

    namespace filters {

        class LangsplitFilter : public boost::iostreams::line_filter {

        public:

            int flags;
            bool print_stats;
            buffered_map bmap;

            std::string output_folder;
            std::string header;
            std::string text_buffer;

            long num_reliable;
            long num_unreliable;

            std::vector<std::string> modes = std::vector<std::string>();

            LangsplitFilter(std::string output_folder_, bool print_stats_);

            virtual ~LangsplitFilter();

            template<typename Sink>
            void close(Sink &snk, BOOST_IOS::openmode which) {
              boost::iostreams::line_filter::close(snk, which);

              string_type line = PrintLanguageStats(flags, header, text_buffer);
              std::streamsize amt = static_cast<std::streamsize>(line.size());
              boost::iostreams::write_if(snk, line.data(), amt);

              bmap.flush();
            }


        private:

            const std::string magic_number = "df6fa1abb58549287111ba8d776733e9";

            std::string do_filter(const std::string &str);

            int get_flag(std::vector<std::string> modes);

            std::string PrintLanguageStats(const int flags, const std::string &header,
                                           const std::string &buffer);

            std::string output_chunk(const std::string buffer, const CLD2::ResultChunk &rc, const std::string header,
                                     const std::string lang_code);

        };

    }

}

#endif //EXTRACTOR_MONO_FILTERS_LANGSPLITFILTER_H
