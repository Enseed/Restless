#include "Precompiled.h"
#include "HttpMethod.h"




std::string HttpMethod::toString()
{
	switch (value())
	{
	case eGET: return "GET";
	case ePUT: return "PUT";
	case ePOST: return "POST";
	case eOPTIONS: return "OPTIONS";
	case eUNKNOWN:
	default:
		return "";
	}
}

HttpMethod::Value HttpMethod::Parse(const std::string &iMethod)
{
	if (iMethod == "GET")
		return eGET;
	if (iMethod == "PUT")
		return ePUT;
	if (iMethod == "POST")
		return ePOST;
	if (iMethod == "DELETE")
		return eDELETE;
	if (iMethod == "OPTIONS")
		return eOPTIONS;
	return eUNKNOWN;
}
