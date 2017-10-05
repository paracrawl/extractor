
#ifndef EXTRACTOR_UTILS_COMMON_H
#define EXTRACTOR_UTILS_COMMON_H

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>


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
          boost::lock_guard<boost::mutex> lock(mutex);
          if (storage.empty()) {
            return T();
          }

          std::string val = storage.back();
          storage.pop_back();
          return val;
        }

        void push(T val) {
          boost::lock_guard<boost::mutex> lock(mutex);
          storage.push_back(val);
        }

        void reverse() {
          boost::lock_guard<boost::mutex> lock(mutex);
          std::reverse(storage.begin(), storage.end());
        }

        int size() {
          boost::lock_guard<boost::mutex> lock(mutex);
          return storage.size();
        }

    private:
        boost::mutex mutex;
        std::vector<T> storage;
    };

    typedef boost::iostreams::filtering_streambuf<boost::iostreams::output> ostreambuf;
    typedef boost::iostreams::stream_buffer<boost::iostreams::file_sink> ofilesink;

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

    class progress {
    public:

        int current_progress;
        int total;

        progress(int total_);

    public:

        void increment();

        void finish();

    private:

        void show_bar();

        boost::mutex mutex;

    };

}

#endif //EXTRACTOR_UTILS_COMMON_H
