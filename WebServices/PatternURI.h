#pragma once

#include <map>
#include <Enseed/Generic/Strings/Strings.h>

class PatternURI
{
public:
	static bool IsSpecialRegex(char c)
	{
		switch (c)
		{
		case '\\':
		case '^':
		case '$':
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
		case '.':
		case '*':
		case '+':
		case '?':
		case '|':
		case '<':
		case '>':
		case '-':
		case '&':
			return true;
		default:
			return false;
		}
	}

	static bool Matches(const std::string patternURI, const std::string uri, std::string *outRemainingPath, std::map<std::string, std::string> *arguments, bool allowSubPath);
	static std::string ToRegexURI(const std::string &patternPath, std::vector<std::string> *outPatterns);


	// substitute {patterns} for * and remove query params
	static std::string ToMatchURI(const std::string &patternPath)
	{
		std::string matchPath;
		bool inPattern = false;

		for (char c : patternPath)
		{
			if (c == '?')
				break;

			if (inPattern)
			{
				if (c == '}')
				{
					inPattern = false;
					matchPath.push_back('*');
				}
			}
			else
			{
				if (c == '{')
					inPattern = true;
				else
					matchPath.push_back(c);
			}
		}

		return matchPath;
	}
};

