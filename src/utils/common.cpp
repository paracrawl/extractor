
#include "common.h"
#include "logging.h"
#include "../3rd_party/utf8/source/utf8.h"

#include <iostream>


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

}