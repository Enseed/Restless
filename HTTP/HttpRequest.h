#pragma once

#include <string>
#include "HttpHeaders.h"
#include "HttpBodyInput.h"
#include "HttpQuery.h"
#include "HttpMethod.h"

struct mg_request_info;

class HttpRequest
{
public:
	HttpRequest(struct mg_connection *conn, const mg_request_info * req_info);

	const std::string& uri() const { return mTargetURI; }
	const std::string& host() const { return mHost; }
	const std::string& protocol() const { return mProtocol; }
	const std::string& hashtag() const { return mHash; }
	const HttpQuery& query() const { return mQuery; }
	const HttpHeaders& headers() const { return mHeaders;	}
	const std::string&  remoteAddr() const { return mRemoteAddr; }
	const HttpBodyInput& body() const { return mBody; }
	const std::string baseURL() const { return protocol() + "://" + host(); }

private:
	HttpMethod mMethod;
	const HttpHeaders mHeaders;
	HttpQuery mQuery;
	HttpBodyInput mBody;
	std::string mRemoteAddr;
	std::string mTargetURI;
	std::string mProtocol;
	std::string mHost;
	std::string mPath;
	std::string mResource;
	std::string mHash;

};

