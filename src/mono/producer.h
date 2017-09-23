
#ifndef EXTRACTOR_PRODUCER_H
#define EXTRACTOR_PRODUCER_H

#include <string>

namespace mono {

    std::stringstream producer_file(std::string path);

    std::stringstream producer_curl(std::string path);

};

#endif //EXTRACTOR_PRODUCER_H
