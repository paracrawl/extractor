
#include "worker.h"
#include "filters/warcfilter.h"
#include "filters/langcollectorfilter.h"
#include "filters/langsplitfilter.h"
#include "../utils/curldownloader.h"
#include "../utils/common.h"
#include "../utils/compression.h"
#include "../utils/logging.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/filter/aggregate.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/null.hpp>

#include <string>


namespace mono {

    void run_worker(shared_vector_string *files_to_process,
                    utils::progress *prog, bool curl, std::string output_folder,
                    utils::compression_option input_compr,
                    utils::compression_option output_compr) {

      while (files_to_process->size() > 0) {
        std::string path = files_to_process->pop();
        if (curl) {
          worker_curl(path, output_folder, input_compr, output_compr);
        } else {
          worker_file(path, output_folder, input_compr, output_compr);
        }

        logging::log_done(output_folder, path);

        prog->increment();

      }
    }

    void
    worker_file(std::string path, std::string output_folder, utils::compression_option input_compr,
                utils::compression_option output_compr) {

      std::ios_base::openmode flags = std::ofstream::in;
      if (input_compr == utils::gzip) {
        flags |= std::ofstream::binary;
      }

      std::ifstream input_file(path, flags);
      if (!boost::filesystem::exists(path)) {
        std::cerr << "File not found!" << std::endl;
        return;
      }

      boost::iostreams::filtering_streambuf<boost::iostreams::input> qin(input_file);
      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      add_decompression(&qout, input_compr);

      qout.push(filters::WARCFilter());
      qout.push(filters::LangsplitFilter(output_folder));
      qout.push(filters::LangCollectorFilter(output_folder, output_compr));
      qout.push(boost::iostreams::null_sink());

      boost::iostreams::copy(qin, qout);

    }

    void worker_curl(std::string url, std::string output_folder, utils::compression_option input_compr,
                     utils::compression_option output_compr) {

      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      add_decompression(&qout, input_compr);

      qout.push(filters::WARCFilter());
      qout.push(filters::LangsplitFilter(output_folder));
      qout.push(filters::LangCollectorFilter(output_folder, output_compr));
      qout.push(boost::iostreams::null_sink());

      std::ostream oqout(&qout);
      HTTPDownloader downloader;
      downloader.download(url, &oqout, output_folder);

    }

}
