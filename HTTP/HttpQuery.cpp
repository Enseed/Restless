#include "Precompiled.h"
#include "HttpQuery.h"
#include <civetweb/include/CivetServer.h>
#include "Enseed/Generic/Strings/Strings.h"



void HttpQuery::parseString(const std::string &queryString)
{
	std::vector<std::string> wParams = seed::Strings::splitstr(queryString, '&');
	for (const std::string &wParam : wParams)
	{
		std::vector<std::string> wKeyValue = seed::Strings::splitstr(wParam, '=', false, 2);
		if (wKeyValue.size() == 1)
			mQueryParams.insert(std::make_pair(wKeyValue[0], std::string()));
		else if (wKeyValue.size() == 2)
		{
			std::string decodedString;
			CivetServer::urlDecode(wKeyValue[1], decodedString);
			mQueryParams.insert(std::make_pair(wKeyValue[0], decodedString));
		}
	}
}
