#pragma once

#include <civetweb/include/civetweb.h>
#include "HttpHeaders.h"
#include "boost/iostreams/stream.hpp"
#include "HttpBodySource.h"

class HttpBodyInput
{
public:
	HttpBodyInput(struct mg_connection *conn, const HttpHeaders &headers)
		: mInputStream(conn)
		, mContentType(headers.value(HttpHeaders::CONTENT_TYPE))
	{}

	const std::istream& bytes() const
	{
		return mInputStream;
	}

	const std::string& contentType() const
	{
		return mContentType;
	}

private:
	std::string mContentType;
	boost::iostreams::stream<HttpBodySource> mInputStream;
};
