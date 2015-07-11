#pragma once

#include <map>

class HttpQuery
{
public:
	typedef std::multimap<std::string, std::string> QueryParams;

private:
	QueryParams mQueryParams;

public:
	HttpQuery(const char *queryString)
	{
		if (queryString != nullptr)
		{ 
			parseString(queryString);
		}
	}

	HttpQuery(const std::string &queryString)
	{
		parseString(queryString);
	}

	void parseString(const std::string &queryString);

public:
	const QueryParams& params() const { return mQueryParams;  }
};

