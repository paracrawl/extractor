
#include "curldownloader.h"

#include <curl/curl.h>

#include <iostream>
#include <sstream>


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

void HTTPDownloader::download(const std::string &url, std::ostream *poqout) {
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, poqout);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

  CURLcode res = curl_easy_perform(curl);

  // Check for errors
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

}
