
#ifndef EXTRACTOR_UTILS_COMMON_H
#define EXTRACTOR_UTILS_COMMON_H

#include <string>
#include <vector>
#include <mutex>


namespace utils {

    enum compression_option {
        none, gzip, null
    };

    compression_option string_to_compression_option(std::string str);


}

#endif //EXTRACTOR_UTILS_COMMON_H
