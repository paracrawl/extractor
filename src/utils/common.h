
#ifndef EXTRACTOR_UTILS_COMMON_H
#define EXTRACTOR_UTILS_COMMON_H

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <utility>


namespace utils {

    enum compression_option {
        none, gzip, lzma, null
    };

    compression_option string_to_compression_option(std::string str);

    std::string compression_option_to_string(compression_option compr);

    std::string get_compression_extension(compression_option compr);

    template<class T>
    class shared_vector {
    public:

        T pop() {
          std::lock_guard<std::mutex> guard(mutex);
          if (storage.empty()) {
            return NULL;
          }

          std::string val = storage.back();
          storage.pop_back();
          return val;
        }

        void push(T val) {
          std::lock_guard<std::mutex> guard(mutex);
          storage.push_back(val);
        }

        void reverse() {
          std::lock_guard<std::mutex> guard(mutex);
          std::reverse(storage.begin(), storage.end());
        }

        int size() {
          std::lock_guard<std::mutex> guard(mutex);
          return storage.size();
        }

    private:
        std::mutex mutex;
        std::vector<T> storage;
    };

    typedef boost::iostreams::filtering_streambuf<boost::iostreams::output> ostreambuf;
    typedef boost::iostreams::stream_buffer<boost::iostreams::file_sink> ofilesink;

    class language_sink {
    public:

        std::string output_folder;
        utils::compression_option compr;

        language_sink(std::string output_folder_, utils::compression_option compr_);

        std::map<std::string, std::pair<ostreambuf *, ofilesink *>> sinkmap;

        void output(std::string const &lang, std::string const &text);

        void close_all();


    private:

        void add_language_sink(std::string lang);

        boost::filesystem::path get_langfile_path(std::string folder, std::string lang);

    };

}

#endif //EXTRACTOR_UTILS_COMMON_H
