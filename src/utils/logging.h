
#ifndef EXTRACTOR_UTILS_LOGGING_H
#define EXTRACTOR_UTILS_LOGGING_H

#define BOOST_LOG_DYN_LINK 1
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)

#include <boost/log/trivial.hpp>

#include <string>


namespace logging {

    void init();

    void log_reliable(std::string output_folder, int num_reliable, int num_unreliable);

    void log_done(std::string output_folder, std::string processed);

    void log_error(std::string output_folder, std::string text);

}

#endif //EXTRACTOR_UTILS_LOGGING_H
