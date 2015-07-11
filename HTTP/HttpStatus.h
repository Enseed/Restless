#pragma once

class HttpStatus
{
public:
	typedef enum 
	{
		eOK = 200,
		eCLIENT_ERROR = 400,
		eNOT_FOUND = 404,
		eSERVER_ERROR = 500
	} Value;

	Value value;

	HttpStatus(Value v);
	static HttpStatus fromInt(int statusCode)
	{
		return HttpStatus((Value)statusCode);
	}

	const char *defaultMessage() const;
};

