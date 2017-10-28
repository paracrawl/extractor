
#ifndef EXTRACTOR_UTILS_CURLDOWNLOADER_H
#define EXTRACTOR_UTILS_CURLDOWNLOADER_H

#include <iostream>
#include <string>
#include <curl/curl.h>


class HTTPDownloader {

public:

    HTTPDownloader();

    ~HTTPDownloader();

    CURLcode download(const std::string &url, std::ostream *poqout);


private:

    void *curl;

};


#endif //EXTRACTOR_UTILS_CURLDOWNLOADER_H
