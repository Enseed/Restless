#pragma once

#include <civetweb/include/civetweb.h>
#include "boost/iostreams/categories.hpp"
#include "boost/iostreams/stream.hpp"

class HttpBodySource : public boost::iostreams::source
{
private:
	mg_connection *mConnection;

public:
	HttpBodySource(struct mg_connection *conn)
		: mConnection(conn)
	{}

	std::streamsize read(char *s, std::streamsize n)
	{
		int nbBytes = mg_read(mConnection, s, n);

		if (nbBytes == 0)
			return -1;	// connection lost
		else if (nbBytes < 0)
			return -1; // end of stream
		else
			return nbBytes;
	}
};

