
#include "curldownloader.h"
#include "logging.h"

#include <curl/curl.h>

#include <iostream>
#include <sstream>
#include <string>


size_t write_data(void *ptr, size_t size, size_t nmemb, void *poqout) {
  (*(std::ostream *) poqout).write((const char *) ptr, nmemb);

  return size * nmemb;
}

HTTPDownloader::HTTPDownloader() {
  curl = curl_easy_init();
}

HTTPDownloader::~HTTPDownloader() {
  curl_easy_cleanup(curl);
}

CURLcode HTTPDownloader::download(const std::string &url, std::ostream *poqout) {
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, poqout);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

  CURLcode res = curl_easy_perform(curl);
  return res;
}
