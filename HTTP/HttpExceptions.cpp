#include "Precompiled.h"
#include "HttpExceptions.h"
#include <stdarg.h>
#include "Generic/Strings/Strings.h"
#include "../Exceptions/Backtrace.h"


HttpClientErrorException::HttpClientErrorException(const HttpStatus &status, const char *message, ...) 
	: HttpException(status)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator=(std::exception(msg));
}


HttpClientErrorException::HttpClientErrorException(const std::exception &cause, const HttpStatus &status, const char *message, ...)
	: HttpException(cause, status)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator=(std::exception(msg));
}

HttpClientErrorException::HttpClientErrorException(const char *message, ...)
	: HttpException(HttpStatus::eCLIENT_ERROR)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}

HttpClientErrorException::HttpClientErrorException(const std::exception &cause, const char *message, ...)
	: HttpException(cause, HttpStatus::eCLIENT_ERROR)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}

HttpClientErrorException::HttpClientErrorException(const HttpStatus &status)
	: HttpException(status, status.defaultMessage())
{

}

HttpClientErrorException::HttpClientErrorException(const std::exception &cause, const HttpStatus &status)
	: HttpException(cause, status, status.defaultMessage())
{

}

HttpClientErrorException::HttpClientErrorException()
	: HttpException(HttpStatus::eCLIENT_ERROR)
{

}

HttpClientErrorException::HttpClientErrorException(const std::exception &cause)
	: HttpException(cause, HttpStatus::eCLIENT_ERROR)
{

}


HttpClientErrorMissingArgumentException::HttpClientErrorMissingArgumentException(const std::string &argumentName)
	: HttpClientErrorException(HttpStatus::eCLIENT_ERROR, seed::Strings::format("argument '%s' was not specified", argumentName.c_str()).c_str())
{

}

HttpClientErrorMissingArgumentException::HttpClientErrorMissingArgumentException(const char *argumentName)
	: HttpClientErrorException(HttpStatus::eCLIENT_ERROR, seed::Strings::format("argument '%s' was not specified", argumentName).c_str())
{

}

HttpClientErrorMissingArgumentException::HttpClientErrorMissingArgumentException() 
	: HttpClientErrorException(HttpStatus::eCLIENT_ERROR)
{

}

HttpException::HttpException(const std::exception &cause, const HttpStatus &statusCode, const char *message) 
	: std::exception(message)
	, mStatusCode(statusCode)
	, mBacktrace(Stacktrace().toString())
	, mCause(cause)
{

}

HttpException::HttpException(const HttpStatus &statusCode, const char *message) 
	: std::exception(message)
	, mStatusCode(statusCode)
	, mBacktrace(Stacktrace().toString())
	, mCause()
{

}

HttpException::HttpException(const std::exception &cause, const HttpStatus &statusCode)
	: std::exception(statusCode.defaultMessage())
	, mStatusCode(statusCode)
	, mBacktrace(Stacktrace().toString())
	, mCause(cause)
{

}

HttpException::HttpException(const HttpStatus &statusCode) 
	: std::exception(statusCode.defaultMessage())
	, mStatusCode(statusCode)
	, mBacktrace(Stacktrace().toString())
	, mCause()
{

}

HttpServerErrorException::HttpServerErrorException() : HttpException(HttpStatus::eSERVER_ERROR)
{

}

HttpServerErrorException::HttpServerErrorException(const std::exception &cause, const char *message, ...) : HttpException(cause, HttpStatus::eSERVER_ERROR, message)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}

HttpServerErrorException::HttpServerErrorException(const char *message, ...) : HttpException(HttpStatus::eSERVER_ERROR, message)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}

HttpServerErrorException::HttpServerErrorException(const HttpStatus &status, const char *message, ...) : HttpException(status, message)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}

HttpServerErrorException::HttpServerErrorException(const std::exception &cause, const HttpStatus &status, const char *message, ...)
	: HttpException(cause, status, message)
{
	char msg[4096];

	va_list args;
	va_start(args, message);
	std::vsprintf(msg, message, args);
	va_end(args);

	std::exception::operator =(std::exception(msg));
}
