
#ifndef EXTRACTOR_PRODUCER_H
#define EXTRACTOR_PRODUCER_H

#include <string>
#include "../utils/common.h"


namespace mono {

    void producer_file(std::string path, std::string output_folder, utils::compression_option input_compr, utils::compression_option output_compr);

    void producer_curl(std::string url, std::string output_folder, utils::compression_option input_compr, utils::compression_option output_compr);

};

#endif //EXTRACTOR_PRODUCER_H
