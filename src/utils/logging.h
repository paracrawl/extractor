
#ifndef EXTRACTOR_UTILS_LOGGING_H
#define EXTRACTOR_UTILS_LOGGING_H

#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>


namespace logging {

    void init() {
      boost::log::add_common_attributes();
      boost::log::add_console_log(std::cout, boost::log::keywords::format =
              (
                      boost::log::expressions::stream
                              << "(" << boost::log::trivial::severity << ") "
                              << "[" << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
                                                                                                            "%Y-%m-%d %H:%M:%S")
                              << "]: "
                              << boost::log::expressions::smessage
              ));

    }

}

#endif //EXTRACTOR_UTILS_LOGGING_H
