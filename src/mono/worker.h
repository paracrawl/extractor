
#ifndef EXTRACTOR_MONO_PRODUCER_H
#define EXTRACTOR_MONO_PRODUCER_H

#include "../utils/common.h"
#include <string>


typedef utils::shared_vector<std::string> shared_vector_string;

namespace mono {

    void run_worker(shared_vector_string *files_to_process,
                    utils::progress *prog, bool curl, std::string output_folder,
                    utils::compression_option input_compr,
                    utils::compression_option output_compr);

    void worker_file(std::string path, std::string output_folder, utils::compression_option input_compr,
                     utils::compression_option output_compr);

    void worker_curl(std::string url, std::string output_folder, utils::compression_option input_compr,
                     utils::compression_option output_compr);

};

#endif //EXTRACTOR_MONO_PRODUCER_H
