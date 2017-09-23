
#ifndef EXTRACTOR_CURLDOWNLOADER_H
#define EXTRACTOR_CURLDOWNLOADER_H

#include <iostream>
#include <string>


class HTTPDownloader {

public:

    HTTPDownloader();

    ~HTTPDownloader();

    void download(const std::string &url, std::ostream *poqout);


private:

    void *curl;

};


#endif //EXTRACTOR_CURLDOWNLOADER_H
