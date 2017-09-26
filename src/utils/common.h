
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

}

#endif //EXTRACTOR_UTILS_COMMON_H
