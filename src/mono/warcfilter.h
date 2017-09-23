
#ifndef EXTRACTOR_WARCFILTER_H
#define EXTRACTOR_WARCFILTER_H

#include <boost/iostreams/filter/line.hpp>
#include <string>


namespace mono {

    class WARCFilter : public boost::iostreams::line_filter {

    public:

        int state;

        WARCFilter() : boost::iostreams::line_filter(true), state(0) {};

    private:

        const std::string warc_guard = "WARC/1.0";
        const std::string uri_guard = "WARC-Target-URI:";
        const std::string magic_number = "df6fa1abb58549287111ba8d776733e9";

        std::string do_filter(const std::string &str);

        std::string make_header(std::string const &uri);

        std::string parse_uri_header(std::string const &str);
    };

}

#endif //EXTRACTOR_WARCFILTER_H
