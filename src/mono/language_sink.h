
#ifndef EXTRACTOR_MONO_LANGUAGE_SINK_H
#define EXTRACTOR_MONO_LANGUAGE_SINK_H

#include "../utils/compression.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <unordered_map>


namespace mono {

    typedef boost::iostreams::filtering_streambuf<boost::iostreams::output> ostreambuf;

    class language_sink {
    public:

        std::string output_folder;
        utils::compression_option compr;

        language_sink(std::string output_folder_, utils::compression_option compr_);

        std::unordered_map<std::string, std::shared_ptr<ostreambuf> > sinkmap;

        void output(std::string const &lang, std::string const &text);


    private:

        void add_language_sink(std::string lang);

        boost::filesystem::path get_langfile_path(std::string folder, std::string lang);

    };

}


#endif //EXTRACTOR_MONO_LANGUAGE_SINK_H
