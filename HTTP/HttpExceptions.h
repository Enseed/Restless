#pragma once

#include <exception>
#include <string>
#include "HttpStatus.h"
#include "Enseed/Generic/Strings/Strings.h"
#include "boost/optional/optional.hpp"

class HttpException : public std::exception
{
private:
	HttpStatus mStatusCode;
	std::string mBacktrace;
	boost::optional<const std::exception&> mCause;

public:
	// make this a polymorphic type to enable RTTI
	virtual ~HttpException() {}

	HttpException(const HttpStatus &statusCode);
	HttpException(const std::exception &cause, const HttpStatus &statusCode);
	HttpException(const HttpStatus &statusCode, const char *message);
	HttpException(const std::exception &cause, const HttpStatus &statusCode, const char *message);

public:
	HttpStatus statusCode() const { return mStatusCode; }
	std::string backtrace() const { return mBacktrace; }
	boost::optional<const std::exception&> cause() const { return mCause; }
};


class HttpClientErrorException : public HttpException
{
public:
	HttpClientErrorException();
	HttpClientErrorException(const std::exception &cause);
	HttpClientErrorException(const HttpStatus &status);
	HttpClientErrorException(const std::exception &cause, const HttpStatus &status);
	HttpClientErrorException(const char *message, ...);
	HttpClientErrorException(const std::exception &cause, const char *message, ...);
	HttpClientErrorException(const HttpStatus &status, const char *message, ...);
	HttpClientErrorException(const std::exception &cause, const HttpStatus &status, const char *message, ...);
};

class HttpClientErrorMissingArgumentException : public HttpClientErrorException
{
public:
	HttpClientErrorMissingArgumentException();
	HttpClientErrorMissingArgumentException(const char *argumentName);
	HttpClientErrorMissingArgumentException(const std::string &argumentName);
};

class HttpServerErrorException : public HttpException
{
public:
	HttpServerErrorException();
	HttpServerErrorException(const std::exception &cause, const char *message, ...);
	HttpServerErrorException(const char *message, ...);
	HttpServerErrorException(const HttpStatus &status, const char *message, ...);
	HttpServerErrorException(const std::exception &cause, const HttpStatus &status, const char *message, ...);
};


class HttpClientErrorInvalidFormatException : public HttpClientErrorException
{
public:
	HttpClientErrorInvalidFormatException()
		: HttpClientErrorException(HttpStatus::eCLIENT_ERROR)
	{}
	HttpClientErrorInvalidFormatException(const std::exception &cause)
		: HttpClientErrorException(cause, HttpStatus::eCLIENT_ERROR)
	{}

	HttpClientErrorInvalidFormatException(const std::exception &cause, const char *argValue)
		: HttpClientErrorException(cause, HttpStatus::eCLIENT_ERROR, seed::Strings::format("Failed to parse: %s", argValue).c_str())
	{}

	HttpClientErrorInvalidFormatException(const std::exception &cause, const std::string &argValue)
		: HttpClientErrorException(cause, HttpStatus::eCLIENT_ERROR, seed::Strings::format("Failed to parse: %s", argValue.c_str()).c_str())
	{}
};

template<class T>
class HttpClientErrorInvalidArgumentException : public HttpClientErrorException
{
public:
	HttpClientErrorInvalidArgumentException()
		: HttpClientErrorException(HttpStatus::eCLIENT_ERROR)
	{}

	HttpClientErrorInvalidArgumentException(const char *argName, const char *argValue)
		: HttpClientErrorException(HttpStatus::eCLIENT_ERROR, seed::Strings::format("Value %s for argument '%s' is not valid. Expected %s", argValue, argName, typeid(T).name))
	{}

	HttpClientErrorInvalidArgumentException(const std::string &argName, const std::string &argValue)
		: HttpClientErrorException(HttpStatus::eCLIENT_ERROR, "Value %s for argument '%s' is not valid. Expected %s", argValue.c_str(), argName.c_str(), typeid(T).name())
	{}
};

template<class T>
class HttpClientErrorOutOfRangeArgumentException : public HttpClientErrorException
{
public:
	HttpClientErrorOutOfRangeArgumentException()
		: HttpClientErrorException(HttpStatus::eCLIENT_ERROR)
	{}

	HttpClientErrorOutOfRangeArgumentException(const char *argName, const char *argValue, const T& min, const T &max)
		: HttpClientErrorException(
			HttpStatus::eCLIENT_ERROR, 
			seed::Strings::format("Value %s for argument '%s' is not valid. Expected a value between %s and %s",
				argValue, 
				argName, 
				seed::Strings::toString(min).c_str(),
				seed::Strings::toString(max).c_str()
			).c_str())
	{}

	HttpClientErrorOutOfRangeArgumentException(const std::string &argName, const std::string &argValue, const T& min, const T &max)
		: HttpClientErrorException(
			HttpStatus::eCLIENT_ERROR, 
			seed::Strings::format("Value %s for argument '%s' is not valid. Expected a value between %s and %s",
				argValue.c_str(),
				argName.c_str(),
				seed::Strings::toString(min).c_str(),
				seed::Strings::toString(max).c_str()
			).c_str())
	{}
};
