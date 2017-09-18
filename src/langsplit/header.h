
#ifndef EXTRACTOR_HEADER_H
#define EXTRACTOR_HEADER_H


#include <string>

#include "string_util.h"

namespace {

    class Header {
    public:
        explicit Header(const std::string &header) {
          for (const auto &value : StringUtil::Split(header, ' ')) {
            if (value.find("tld:") == 0) {
              tld_ = value.substr(4);
            } else if (value.find("uri:") == 0) {
              uri_ = value.substr(4);
            } else if (value.find("encoding:") == 0) {
              encoding_ = value.substr(9);
            }
          }
        }

        const std::string get_tld() const { return tld_; }

        const std::string get_uri() const { return uri_; }

        const std::string get_encoding() const { return encoding_; }

    private:
        std::string uri_;
        std::string tld_;
        std::string encoding_;
    };

}  // namespace


#endif //EXTRACTOR_HEADER_H
