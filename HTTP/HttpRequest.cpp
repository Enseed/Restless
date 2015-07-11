#include "Precompiled.h"
#include "HttpRequest.h"




HttpRequest::HttpRequest(struct mg_connection *conn, const mg_request_info * req_info) 
	: mMethod(req_info->request_method)
	, mHeaders(req_info->http_headers, req_info->num_headers)
	, mQuery(req_info->query_string)
	, mBody(conn, mHeaders)
	, mRemoteAddr(req_info->remote_addr)
	, mTargetURI(req_info->uri)
{
	mHost = mHeaders.value("Host");
	mProtocol = req_info->is_ssl ? "https" : "http";
}
