
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

void HTTPDownloader::download(const std::string &url, std::ostream *poqout, std::string output_folder) {
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, poqout);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

  CURLcode res = curl_easy_perform(curl);

  // Check for errors
  if (res != CURLE_OK) {
    std::string error_text = "Failed to download: " + url + " with error: " + curl_easy_strerror(res);
    logging::log_error(output_folder, error_text);
  }

}
