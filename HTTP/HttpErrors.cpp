#include "Precompiled.h"

#include "HttpErrors.h"
#include "HttpExceptionErrors.h"

void HttpErrors::createDTOFromException(const std::exception &ex, ErrorDTO *outDTO)
{
	outDTO->id = boost::uuids::random_generator()();
	outDTO->time = boost::posix_time::second_clock::local_time();
	outDTO->message = ex.what();
	outDTO->type = typeid(ex).name();
}

void HttpErrors::createDTOFromException(const HttpException &ex, ErrorDTO *outDTO)
{
	outDTO->id = boost::uuids::random_generator()();
	outDTO->time = boost::posix_time::second_clock::local_time();
	outDTO->message = ex.what();
	outDTO->stack = seed::Strings::splitstr(ex.backtrace(), '\n');
	outDTO->type = typeid(ex).name();
	if (ex.cause())
	{
		const std::exception &cause = ex.cause().get();
		outDTO->cause = new ErrorDTO();
		if (seed::RTTI::instanceof<HttpException>(cause))
			createDTOFromException((const HttpException&)cause, outDTO->cause.ptr());
		else
			createDTOFromException(cause, outDTO->cause.ptr());
	}
}

seed::AutoPtr<HttpResult> HttpErrors::createFromException(const std::exception &ex)
{
	const HttpException *httpEx = dynamic_cast<const HttpException*>(&ex);
	if (seed::RTTI::instanceof<HttpException>(ex))
	{
		return new HttpExceptionError<HttpException>(static_cast<const HttpException&>(ex));
	}
	else
	{
		return new HttpExceptionError<std::exception>(static_cast<const std::exception&>(ex));
	}
}
