
#include "producer.h"
#include "warcfilter.h"
#include "../langsplit/langsplitfilter.h"
#include "../utils/curldownloader.h"
#include "../utils/common.h"
#include "../utils/logging.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>


namespace mono {

    void producer_file(std::stringstream &output, std::string path, utils::compression_option compr) {
      LOG_INFO << "Local files will be processed. ";

      std::ifstream input_file(path, std::ios_base::in | std::ios_base::binary);
      if (!boost::filesystem::exists(path)) {
        std::cerr << "File not found!" << std::endl;
        return;
      }

      boost::iostreams::filtering_streambuf<boost::iostreams::input> qin(input_file);
      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      if (compr == utils::gzip) {
        qout.push(boost::iostreams::gzip_decompressor());
      }

      qout.push(WARCFilter());
      qout.push(LangsplitFilter());
      qout.push(output);

      boost::iostreams::copy(qin, qout);

    }

    void producer_curl(std::stringstream &output, std::string url, utils::compression_option compr) {
      LOG_INFO << "Using curl to download remote files. ";

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

    }

}
