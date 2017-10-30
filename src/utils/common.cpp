
#include "common.h"
#include "logging.h"
#include "../3rd_party/utf8/source/utf8.h"

#include <iostream>
#include <exception>


namespace utils {

    progress::progress(int total_) : current_progress(0), total(total_) {
      std::cout << std::endl;
      show_bar();
    }

    void progress::increment() {
      boost::lock_guard<boost::mutex> lock(mutex);
      ++current_progress;
      show_bar();
    }

    void progress::show_bar() {
      int bar_width = 70;

      std::cout << " " << current_progress << "/" << total << " [";
      int pos = bar_width * current_progress / float(total);
      for (int i = 0; i < bar_width; ++i) {
        if (i < pos)
          std::cout << "=";
        else if (i == pos)
          std::cout << ">";
        else
          std::cout << " ";
      }
      std::cout << "] " << int(current_progress / float(total) * 100.0) << " %\r";
      std::cout.flush();
    };

    void progress::finish() {
      std::cout << std::endl;
      LOG_INFO << "Done.";
    }

    void fix_utf8_string(std::string &str) {
      std::string temp;
      utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
      str = temp;
    }

    parse_uri::parse_uri(const std::string &url) : uri_scheme(""), uri_domain(""), uri_tld(""), uri_path("") {
      try {
        int scheme_end = parse_uri::parse_scheme(0, url);
        int domain_end = parse_domains(scheme_end, url);
        parse_path(domain_end, url);
      } catch (std::exception &e) {
        std::cerr << "Error in parse_uri: " << e.what() << std::endl;
        uri_domain = url;
      }

    }

    int parse_uri::parse_scheme(int start, const std::string &uri) {
      std::string uri_stripped = uri.substr(start, uri.length() - start);
      int found = uri.find("://");
      if (found != static_cast<int>(std::string::npos)) {
        uri_scheme = uri.substr(0, found);
        return found + 3;
      }

      return 0;
    }

    int parse_uri::parse_domains(int start, const std::string &uri) {
      // parse authority
      int found_authority = uri.find("/", start);
      if (found_authority == static_cast<int>(std::string::npos))
        found_authority = uri.length();
      std::string domain = uri.substr(start, found_authority - start);

      // remove user information
      int found_user = domain.find("@");
      if (found_user != static_cast<int>(std::string::npos)) {
        domain = domain.substr(found_user + 1, domain.length() - found_user - 1);
      }

      // remove port
      int found_port = domain.find_last_of(":");
      if (found_port != static_cast<int>(std::string::npos)) {
        domain = domain.substr(0, found_port);
      }

      // parse tld
      int found_tld_start = domain.find_last_of(".");
      if (found_tld_start == static_cast<int>(std::string::npos))
        found_tld_start = 0;
      else
        ++found_tld_start;
      std::string tld = domain.substr(found_tld_start, domain.length() - found_tld_start);

      // save
      uri_domain = domain;
      uri_tld = tld;

      return found_authority + 1;
    }

    int parse_uri::parse_path(int start, const std::string &uri) {
      if (start >= static_cast<int>(uri.length()))
        return uri.length();

      // parse path
      int found_path = uri.find("?", start);
      if (found_path == static_cast<int>(std::string::npos))
        found_path = uri.length();
      uri_path = uri.substr(start, found_path - start);

      return found_path;
    }

    const std::string &parse_uri::get_scheme() const {
      return uri_scheme;
    }

    const std::string &parse_uri::get_domain() const {
      return uri_domain;
    }

    const std::string &parse_uri::get_tld() const {
      return uri_tld;
    }

    const std::string &parse_uri::get_path() const {
      return uri_path;
    }

}