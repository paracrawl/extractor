
#include "producer.h"
#include "warcfilter.h"
#include "langcollector.h"
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
#include <boost/iostreams/device/null.hpp>


namespace mono {

    void
    producer_file(std::string path, std::string output_folder, utils::compression_option input_compr, utils::compression_option output_compr) {
      LOG_INFO << "Local files will be processed. ";

      std::ifstream input_file(path, std::ios_base::in | std::ios_base::binary);
      if (!boost::filesystem::exists(path)) {
        std::cerr << "File not found!" << std::endl;
        return;
      }

      boost::iostreams::filtering_streambuf<boost::iostreams::input> qin(input_file);
      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      if (input_compr == utils::gzip) {
        qout.push(boost::iostreams::gzip_decompressor());
      }

      qout.push(WARCFilter());
      qout.push(LangsplitFilter());
      qout.push(LangCollectorFilter(output_folder, output_compr));
      qout.push(boost::iostreams::null_sink());

      boost::iostreams::copy(qin, qout);

    }

    void producer_curl(std::string url, std::string output_folder, utils::compression_option input_compr, utils::compression_option output_compr) {
      LOG_INFO << "Using curl to download remote files. ";

      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      if (input_compr == utils::gzip) {
        qout.push(boost::iostreams::gzip_decompressor());
      }

      qout.push(WARCFilter());
      qout.push(LangsplitFilter());
      qout.push(LangCollectorFilter(output_folder, output_compr));
      qout.push(boost::iostreams::null_sink());

      std::ostream oqout(&qout);
      HTTPDownloader downloader;
      std::cout << url << std::endl;
      downloader.download(url, &oqout);

    }

}
