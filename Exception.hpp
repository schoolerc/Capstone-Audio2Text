#include <exception>
#include <boost/exception/all.hpp>

//The host name of any attempted network communications
typedef boost::error_info<struct tag_errhost, std::string> errhost_info;

//The high level action that failed
typedef boost::error_info<struct tag_erraction, std::string> erraction_info;

//The serialized http request sent
typedef boost::error_info<struct tag_errrequest, std::string> errrequest_info;

//The raw http response received
typedef boost::error_info<struct tag_errresponse, std::string> errresponse_info;