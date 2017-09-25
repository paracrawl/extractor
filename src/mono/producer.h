
#ifndef EXTRACTOR_PRODUCER_H
#define EXTRACTOR_PRODUCER_H

#include <string>
#include "../utils/common.h"


namespace mono {

    std::stringstream producer_file(std::string path, utils::compression_option compr);

    std::stringstream producer_curl(std::string path, utils::compression_option compr);

};

#endif //EXTRACTOR_PRODUCER_H
