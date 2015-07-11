#pragma once

#include "HTTP/HttpResult.h"
#include "JSON/JSON.h"

class HttpDTOResult : public HttpResult
{
public:
	template<typename DTO>
	static HttpResult* createFromDTO(const DTO &dto, const std::string &format)
	{
		if (format == HttpMediaType::APPLICATION_JSON)
		{
			HttpResult *result = new HttpResult();
			result->setStatus(HttpStatus::eOK);
			result->setBytes(JSONParser::toJSON(dto), format);
			return result;
		}

		return nullptr;
	}

	template<typename DTO>
	static HttpResult* createFromDTO(const std::vector<DTO> &dto, const std::string &format)
	{
		if (format == HttpMediaType::APPLICATION_JSON)
		{
			HttpResult *result = new HttpResult();
			result->setStatus(HttpStatus::eOK);
			result->setBytes(JSONParser::toJSON(dto), format);
			return result;
		}

		return nullptr;
	}
};

