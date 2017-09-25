
#include "producer.h"
#include "warcfilter.h"
#include "../langsplit/langsplitfilter.h"
#include "../utils/curldownloader.h"
#include "../utils/common.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>


namespace mono {

    std::stringstream producer_file(std::string path, utils::compression_option compr) {
      std::ifstream input_file(path, std::ios_base::in | std::ios_base::binary);
      if (!boost::filesystem::exists(path)) {
        std::cerr << "File not found!" << std::endl;
        return std::stringstream();
      }

      std::stringstream output;
      boost::iostreams::filtering_streambuf<boost::iostreams::input> qin(input_file);
      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      if (compr == utils::gzip) {
        qout.push(boost::iostreams::gzip_decompressor());
      }

      qout.push(WARCFilter());
      qout.push(LangsplitFilter());
      qout.push(output);

      boost::iostreams::copy(qin, qout);

      return output;
    }

    std::stringstream producer_curl(std::string url, utils::compression_option compr) {
      std::stringstream output;
      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      if (compr == utils::gzip) {
        qout.push(boost::iostreams::gzip_decompressor());
      }

      qout.push(WARCFilter());
      qout.push(LangsplitFilter());
      qout.push(output);

      std::ostream oqout(&qout);
      HTTPDownloader downloader;
      downloader.download(url, &oqout);

      return output;
    }

}
