
#ifndef EXTRACTOR_PRODUCER_H
#define EXTRACTOR_PRODUCER_H

#include <string>
#include "../utils/common.h"


namespace mono {

    void producer_file(std::stringstream &ss, std::string path, utils::compression_option compr);

    void producer_curl(std::stringstream &ss, std::string path, utils::compression_option compr);

};

#endif //EXTRACTOR_PRODUCER_H
