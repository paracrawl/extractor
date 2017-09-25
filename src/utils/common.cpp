
#include "common.h"

namespace utils {

    compression_option string_to_compression_option(std::string str) {
      if (str == "gzip") return gzip;
      if (str == "none") return gzip;

      return null;
    }

}