#pragma once

#include "HttpStatus.h"
#include "HttpHeaders.h"

class HttpResult
{
public:
	HttpResult();

	HttpStatus mStatusCode;
	std::string mStatus;
	std::string mBytes;
	std::string mContentType;
	HttpHeaders mHeaders;

	void setStatus(HttpStatus code);
	void setStatus(HttpStatus code, const std::string &message);
	void setBytes(const std::string &bytes, const std::string &format);

	HttpStatus statusCode() const;
	std::string status() const;
	const std::string& bytes() const;
	std::string contentType() const;
	const HttpHeaders&  headers() const;
};


