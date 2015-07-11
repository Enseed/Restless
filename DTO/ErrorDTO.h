#ifndef __ERRORDTO__
#define __ERRORDTO__

#include <string>
#include <Enseed/Generic/CountPtr/CountPtr.h>
#include <boost/optional.hpp>

#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>

struct ErrorDTO
{
	boost::posix_time::ptime time;
	boost::uuids::uuid id;
	std::string type;
	std::string message;
	boost::optional<std::vector<std::string>> stack;
	seed::CountPtr<ErrorDTO> cause;
};


#endif // __ERRORDTO__
