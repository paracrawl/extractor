
#include "compression.h"

#ifdef WITH_LZMA
#include <boost/iostreams/filter/lzma.hpp>
#endif

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>


namespace utils {

    compression_option string_to_compression_option(std::string str) {
      if (str == "gzip") return gzip;
      if (str == "bzip2") return bzip2;
      if (str == "zlib") return zlib;
      if (str == "lzma") return lzma;
      if (str == "none") return none;

      return null;
    }

    std::string compression_option_to_string(compression_option compr) {
      if (compr == gzip) return "gzip";
      if (compr == bzip2) return "bzip2";
      if (compr == zlib) return "zlib";
      if (compr == lzma) return "lzma";
      if (compr == none) return "none";

      return "";
    }

    std::string get_compression_extension(compression_option compr) {
      if (compr == gzip) return "gz";
      if (compr == bzip2) return "bz2";
      if (compr == zlib) return "zz";
      if (compr == lzma) return "xz";
      if (compr == none) return "out";

      return "";
    }

    void add_compression(std::shared_ptr<boost::iostreams::filtering_streambuf<boost::iostreams::output>> osb,
                         utils::compression_option compr) {

      if (compr == utils::gzip) {
        osb->push(boost::iostreams::gzip_compressor());
      }

      if (compr == utils::bzip2) {
        osb->push(boost::iostreams::bzip2_compressor());
      }

      if (compr == utils::zlib) {
        osb->push(boost::iostreams::zlib_compressor());
      }

      #ifdef WITH_LZMA
      if (compr == utils::lzma) {
        osb->push(boost::iostreams::lzma_compressor());
      }
      #endif
    }

    void add_decompression(boost::iostreams::filtering_streambuf<boost::iostreams::output> *osb,
                           utils::compression_option compr) {

      if (compr == utils::gzip) {
        osb->push(boost::iostreams::gzip_decompressor());
      }

      if (compr == utils::bzip2) {
        osb->push(boost::iostreams::bzip2_decompressor());
      }

      if (compr == utils::zlib) {
        osb->push(boost::iostreams::zlib_decompressor());
      }

      #ifdef WITH_LZMA
      if (compr == utils::lzma) {
        osb->push(boost::iostreams::lzma_decompressor());
      }
      #endif
    }


}