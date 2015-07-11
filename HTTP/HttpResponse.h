#pragma once

#include "HttpStatus.h"
#include "HttpHeaders.h"
#include "HttpBodyOutput.h"

class HttpResult;
struct mg_connection;

class HttpResponse
{
private:
	mg_connection *mConnection;

public:
	HttpResponse(struct mg_connection *conn);

public:
	void send(const HttpResult &result);

private:
	static void sendGenericHeader(mg_connection *connection, const HttpResult &result);
	static void sendHeaders(mg_connection *connection, const HttpResult &result);
	static void sendBody(mg_connection *connection, const HttpResult &result);

	static const char *KEEP_ALIVE;
	static const char *CLOSE;

	/* HTTP 1.1 assumes keep alive if "Connection:" header is not set
	* This function must tolerate situations when connection info is not
	* set up, for example if request parsing failed. */
	static bool shouldKeepAlive(const HttpResult &result);

	/* Convert time_t to a string. According to RFC2616, Sec 14.18, this must be
	* included in all responses other than 100, 101, 5xx. */
	static void TimeString(char *buf, size_t buf_len, time_t *t);

	static void mg_strlcpy(register char *dst, register const char *src, size_t n);
};

