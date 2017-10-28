
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

    void fix_utf8_string(std::string& str);

}

#endif //EXTRACTOR_UTILS_COMMON_H
