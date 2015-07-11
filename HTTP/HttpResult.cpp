#include "Precompiled.h"
#include "HttpResult.h"


const HttpHeaders& HttpResult::headers() const
{
	return mHeaders;
}

std::string HttpResult::contentType() const
{
	return mContentType;
}

const std::string& HttpResult::bytes() const
{
	return mBytes;
}

void HttpResult::setBytes(const std::string &bytes, const std::string &format)
{
	mBytes = bytes;
	mContentType = format;
}

std::string HttpResult::status() const
{
	return mStatus;
}

HttpStatus HttpResult::statusCode() const
{
	return mStatusCode;
}

void HttpResult::setStatus(HttpStatus code, const std::string &message)
{
	mStatusCode = code;
	mStatus = message;
}

void HttpResult::setStatus(HttpStatus code)
{
	setStatus(code, code.defaultMessage());
}

HttpResult::HttpResult() : mStatusCode(HttpStatus::eOK)
{

}
