#pragma once

#include <string>
#include <map>
#include <civetweb/include/civetweb.h>


/************************************************************************/
/* Comparator for case-insensitive comparison in STL assos. containers  */
/************************************************************************/
struct ci_less : std::binary_function < std::string, std::string, bool >
{
	// case-independent (ci) compare_less binary function
	struct nocase_compare : public std::binary_function < unsigned char, unsigned char, bool >
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const {
			return tolower(c1) < tolower(c2);
		}
	};
	bool operator() (const std::string & s1, const std::string & s2) const {
		return std::lexicographical_compare
			(s1.begin(), s1.end(),   // source range
			s2.begin(), s2.end(),   // dest range
			nocase_compare());  // comparison
	}
};

class HttpHeaders
{
public:
	typedef std::multimap<std::string, std::string, ci_less> HeaderValues;

	const std::string& value(const std::string &key) const
	{
		auto found = mHeaderValues.lower_bound(key);
		if (found != mHeaderValues.end())
		{
			return found->second;
		}
		return EMPTY;
	}

	const HeaderValues& getValues() const
	{
		return mHeaderValues;
	}

	void setValue(const std::string &key, const std::string &value)
	{
		mHeaderValues.erase(key);
		mHeaderValues.insert(std::make_pair(key, value));
	}

private:
	static std::string EMPTY;
	HeaderValues mHeaderValues;

public:
	HttpHeaders()
	{}

	HttpHeaders(const mg_request_info::mg_header *iHeaders, int iNumHeaders)
	{
		for (int i = 0; i < iNumHeaders; ++i)
		{
			std::string key = iHeaders[i].name;
			std::string value = iHeaders[i].value;
			mHeaderValues.insert(std::make_pair(key, value));
		}
	}

	static const std::string CONTENT_TYPE;
	static const std::string CONTENT_LENGTH;
};


