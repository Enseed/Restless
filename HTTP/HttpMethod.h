#pragma once

#include <string>

class HttpMethod
{
public:
	typedef enum {
		eGET,
		ePUT,
		ePOST,
		eDELETE,
		eOPTIONS,
		eUNKNOWN
	} Value;

private:
	Value mValue;

public:
	Value value() { return mValue; }
	
	void setValue(Value iValue) { mValue = iValue; }
	
	HttpMethod(Value iValue)
	{
		setValue(iValue);
	}

	HttpMethod(const std::string &stringValue)
	{
		setValue(Parse(stringValue));
	}

	std::string toString();

	static Value Parse(const std::string &iMethod);
};

