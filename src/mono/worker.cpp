
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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <curl/curl.h>

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
      logging::log_done(output_folder, path);

    }

    void worker_curl(std::string url, std::string output_folder, utils::compression_option input_compr,
                     utils::compression_option output_compr) {

      boost::iostreams::filtering_streambuf<boost::iostreams::output> qout;

      add_decompression(&qout, input_compr);

      qout.push(filters::WARCFilter());
      qout.push(filters::LangsplitFilter(output_folder));
      qout.push(filters::LangCollectorFilter(output_folder, output_compr));
      qout.push(boost::iostreams::null_sink());

      // split input on a single space
      std::vector<std::string> parsed_urls;
      boost::algorithm::split(parsed_urls, url, boost::algorithm::is_any_of(" "));

      std::size_t i = 0;
      std::ostream oqout(&qout);
      HTTPDownloader downloader;
      CURLcode res = downloader.download(parsed_urls.at(i), &oqout);

      // download from other sources if failed
      while (res != CURLE_OK) {
        std::string error_text =
                "Failed to download from: " + parsed_urls.at(i) + " with error: " + curl_easy_strerror(res);
        logging::log_error(output_folder, error_text);

        if (++i >= parsed_urls.size()) break;

        res = downloader.download(parsed_urls.at(i), &oqout);
      }

      // Not found in any source
      if (res != CURLE_OK) {
        std::string error_text = "CURL ERROR: " + parsed_urls.at(0) + " failed to process!";
        logging::log_error(output_folder, error_text);
      } else {
        logging::log_done(output_folder, parsed_urls.at(i));
      }

    }

}
