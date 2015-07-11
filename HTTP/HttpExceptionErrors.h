#pragma once

#include "JSON/JSON.h"
#include "HttpResult.h"
#include "HttpMediaType.h"
#include "HttpExceptionErrors.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>


#include <Enseed/Generic/Strings/Strings.h>
#include <Enseed/Generic/Base/RTTI.h>

#include "C:\Work\Enseed_svn\trunk\Applications\Mediatheque\win32\Mediatek\RESTSchema\schema\generated\ErrorDTO.h"

#include "HttpErrors.h"
#include "HttpExceptions.h"


template<class TEX>
class HttpExceptionError : public HttpResult
{
public:
	HttpExceptionError(const TEX &iEx)
	{
		const std::exception &ex = iEx;
		ErrorDTO dto;
		HttpErrors::createDTOFromException(ex, &dto);

		setStatus(HttpStatus::eSERVER_ERROR);
		setBytes(JSONParser::toJSON(dto), HttpMediaType::APPLICATION_JSON);
	}
};

template<>
class HttpExceptionError<HttpException> : public HttpResult
{
public:
	HttpExceptionError(const HttpException &ex)
	{
		ErrorDTO dto;
		HttpErrors::createDTOFromException(ex, &dto);
		setStatus(ex.statusCode(), ex.what());
		setBytes(JSONParser::toJSON(dto), HttpMediaType::APPLICATION_JSON);
	}
};


