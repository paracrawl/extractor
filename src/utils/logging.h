
#ifndef EXTRACTOR_UTILS_LOGGING_H
#define EXTRACTOR_UTILS_LOGGING_H

#define BOOST_LOG_DYN_LINK 1
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)

#include <boost/log/trivial.hpp>


namespace logging {

    void init();

}

#endif //EXTRACTOR_UTILS_LOGGING_H
