
#ifndef EXTRACTOR_UTILS_COMPRESSION_H
#define EXTRACTOR_UTILS_COMPRESSION_H

#include <boost/iostreams/filtering_stream.hpp>
#include <string>


namespace utils {

    enum compression_option {
        none, gzip, bzip2, zlib, lzma, null
    };

    compression_option string_to_compression_option(std::string str);

    std::string compression_option_to_string(compression_option compr);

    std::string get_compression_extension(compression_option compr);

    void add_compression(std::shared_ptr<boost::iostreams::filtering_streambuf<boost::iostreams::output>> osb,
                         utils::compression_option compr);

    void add_decompression(boost::iostreams::filtering_streambuf<boost::iostreams::output> *osb,
                           utils::compression_option compr);

}

#endif //EXTRACTOR_UTILS_COMPRESSION_H
