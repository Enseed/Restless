#pragma once

#include <civetweb/include/civetweb.h>
#include "boost/iostreams/categories.hpp"
#include "boost/iostreams/stream.hpp"

class HttpBodySink : public boost::iostreams::sink
{
private:
	mg_connection *mConnection;

public:
	HttpBodySink(struct mg_connection *conn)
		: mConnection(conn)
	{}

	std::streamsize write(const char_type *s, std::streamsize n)
	{
		int nbBytes = mg_write(mConnection, s, n);

		if (nbBytes == 0)
			return -1;	// connection lost
		else if (nbBytes < 0)
			return -1; // end of stream
		else
			return nbBytes;
	}
};

