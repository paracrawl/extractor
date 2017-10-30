
#ifndef EXTRACTOR_MONO_BUFFERED_MAP_H
#define EXTRACTOR_MONO_BUFFERED_MAP_H

#include <iostream>
#include <fstream>
#include <map>

namespace mono {

    class buffered_map {
    public:

        std::string output_folder;

        unsigned long buffer_size;
        int file_no;

        std::map<std::pair<std::string, std::string>, long> domain_map;

        buffered_map(std::string output_folder_, unsigned long buffer_size_);

        void add(std::pair<std::string, std::string> key, long value);

        void flush();

        size_t size();


    private:

    };

}

#endif //EXTRACTOR_MONO_BUFFERED_MAP_H
