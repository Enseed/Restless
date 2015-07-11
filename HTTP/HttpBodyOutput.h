#pragma once

#include "boost/iostreams/stream.hpp"
#include "HttpBodySink.h"
#include <string>

class HttpBodyOutput
{
public:
	HttpBodyOutput(struct mg_connection *conn)
		: mOutputStream(conn)
	{}

	void setContent(const std::string &content, const std::string &contentType)
	{
		mContentType = contentType;
		mContent = content;
	}

	const std::string& contentType()
	{
		return mContentType;
	}

	size_t contentLength()
	{
		return mContent.length();
	}

	void *bytes()
	{
		return &mContent[0];
	}

private:
	std::string mContentType;
	std::string mContent;
	boost::iostreams::stream<HttpBodySink> mOutputStream;
};



