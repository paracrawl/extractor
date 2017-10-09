
#ifndef EXTRACTOR_UTILS_CURLDOWNLOADER_H
#define EXTRACTOR_UTILS_CURLDOWNLOADER_H

#include <iostream>
#include <string>


class HTTPDownloader {

public:

    HTTPDownloader();

    ~HTTPDownloader();

    void download(const std::string &url, std::ostream *poqout, std::string output_folder);


private:

    void *curl;

};


#endif //EXTRACTOR_UTILS_CURLDOWNLOADER_H
