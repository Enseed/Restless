#include "Precompiled.h"
#include "PatternURI.h"
#include "boost/regex.hpp"
#include <civetweb/include/CivetServer.h>




std::string PatternURI::ToRegexURI(const std::string &patternPath, std::vector<std::string> *outPatterns)
{
	bool inPattern = false;
	std::string regexPath;
	std::string patternName;
	for (char c : patternPath)
	{
		if (inPattern)
		{
			if (c == '}')
			{
				inPattern = false;
				regexPath.push_back('(');
				regexPath.push_back('[');
				regexPath.push_back('^');
				regexPath.push_back('/');
				regexPath.push_back(']');
				regexPath.push_back('+');
				regexPath.push_back(')');
				if (outPatterns)
					outPatterns->push_back(patternName);

				patternName.clear();
			}
			else
			{
				patternName.push_back(c);
			}
		}
		else
		{
			if (c == '{')
				inPattern = true;
			else
			{
				if (IsSpecialRegex(c))
					regexPath.push_back('\\');

				regexPath.push_back(c);
			}
		}
	}

	return regexPath;
}

bool PatternURI::Matches(const std::string patternURI, const std::string uri, std::string *outRemainingPath, std::map<std::string, std::string> *arguments, bool allowSubPath)
{
	std::vector<std::string> uriQuery = seed::Strings::splitstr(uri, '?', false, 2);
	if (uriQuery.empty())
		return false;

	std::string uriPart = uriQuery[0];
	std::string uriQueryPart = uriQuery.size() > 1 ? uriQuery[1] : std::string();

	std::vector<std::string> patternQuery = seed::Strings::splitstr(patternURI, '?', false, 2);
	if (patternQuery.empty())
		return false;

	std::string patternPart = patternQuery[0];
	std::string patternQueryPart = patternQuery.size() > 1 ? patternQuery[1] : std::string();

	std::vector<std::string> patternNames;
	std::string regexStr = "^" + PatternURI::ToRegexURI(patternPart, &patternNames);

	bool matchSubPath = false;
	if (patternQuery.size() == 1 && allowSubPath)
	{
		matchSubPath = true;
		regexStr += "(.*)";
	}
	else
	{
		regexStr += "$";
	}

	boost::regex expr(regexStr);
	boost::smatch what;
	if (boost::regex_search(uri, what, expr))
	{
		int max = (int)what.size();

		if (matchSubPath)
		{
			max = max - 1;
			*outRemainingPath = what[max];
		}

		for (int i = 1; i < max; ++i)
		{
			std::string match(what[i].first, what[i].second);
			std::string decodedMatch;
			CivetServer::urlDecode(match, decodedMatch);
			(*arguments)[patternNames[i - 1]] = decodedMatch;
		}
		return true;
	}

	return false;
}
