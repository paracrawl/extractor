
#ifndef EXTRACTOR_HEADER_H
#define EXTRACTOR_HEADER_H

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


#endif //EXTRACTOR_HEADER_H
