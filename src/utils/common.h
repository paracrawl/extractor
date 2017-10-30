
#ifndef EXTRACTOR_UTILS_COMMON_H
#define EXTRACTOR_UTILS_COMMON_H

#include "compression.h"
#include <boost/thread.hpp>
#include <string>
#include <vector>


namespace utils {

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

    class parse_uri {
    public:

        std::string uri_scheme;
        std::string uri_domain;
        std::string uri_tld;
        std::string uri_path;

        const std::string &get_scheme() const;

        const std::string &get_domain() const;

        const std::string &get_path() const;

        const std::string &get_tld() const;

        parse_uri(const std::string &uri);


    private:

        int parse_scheme(int start, const std::string &uri);

        int parse_domains(int start, const std::string &uri);

        int parse_path(int start, const std::string &uri);

    };

    void fix_utf8_string(std::string &str);

}

#endif //EXTRACTOR_UTILS_COMMON_H
