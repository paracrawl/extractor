
#ifndef EXTRACTOR_LANGCOLLECTOR_H
#define EXTRACTOR_LANGCOLLECTOR_H

#include "../utils/common.h"

#include <boost/iostreams/filter/line.hpp>
#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/copy.hpp>

#include <iostream>
#include <string>
#include <map>


namespace mono {

    class LangCollectorFilter : public boost::iostreams::line_filter {

    public:

        std::string header;
        std::string text_buffer;
        utils::language_sink ls;

        LangCollectorFilter(std::string output_folder_, utils::compression_option compr_);

        template<typename Sink>
        void close(Sink &snk, BOOST_IOS::openmode which) {
          boost::iostreams::line_filter::close(snk, which);

          ls.close_all();
        }


    private:

        const std::string magic_number = "df6fa1abb58549287111ba8d776733e9";
        const std::string language_guard = "language:";

        std::string do_filter(const std::string &str);

        std::string parse_language(std::string const &str);

        std::string find_language(std::string const &str);

        void output_to_langsink(std::string const &lang, std::string const &text);

    };

}


#endif //EXTRACTOR_LANGCOLLECTOR_H
