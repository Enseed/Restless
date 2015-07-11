#include "Precompiled.h"

#include "HttpResponse.h"
#include "HttpResult.h"

const char *HttpResponse::KEEP_ALIVE = "keep-alive";
const char *HttpResponse::CLOSE = "close";



void HttpResponse::mg_strlcpy(register char *dst, register const char *src, size_t n)
{
	for (; *src != '\0' && n > 1; n--) {
		*dst++ = *src++;
	}
	*dst = '\0';
}

void HttpResponse::TimeString(char *buf, size_t buf_len, time_t *t)
{
	struct tm *tm;

	tm = gmtime(t);
	if (tm != NULL) {
		strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", tm);
	}
	else {
		mg_strlcpy(buf, "Thu, 01 Jan 1970 00:00:00 GMT", buf_len);
		buf[buf_len - 1] = '\0';
	}
}

bool HttpResponse::shouldKeepAlive(const HttpResult &result)
{
	return true;
}

void HttpResponse::sendBody(mg_connection *connection, const HttpResult &result)
{
	mg_printf(connection,
		"%s: %s\r\n"
		"%s: %d\r\n\r\n",
		HttpHeaders::CONTENT_TYPE.c_str(), result.contentType().c_str(),
		HttpHeaders::CONTENT_LENGTH.c_str(), result.bytes().size());

	mg_write(connection, &result.bytes()[0], result.bytes().size());
}

void HttpResponse::sendHeaders(mg_connection *connection, const HttpResult &result)
{
	auto values = result.headers().getValues();
	for (auto value : values)
	{
		const std::string &k = value.first;
		const std::string &v = value.second;
		mg_printf(connection,
			"%s: %s\r\n",
			k.c_str(), v.c_str());
	}
}

void HttpResponse::sendGenericHeader(mg_connection *connection, const HttpResult &result)
{
	char date[64];
	time_t curtime = time(NULL);
	TimeString(date, sizeof(date), &curtime);

	mg_printf(connection,
		"HTTP/1.1 %d %s\r\n"
		"Date: %s\r\n"
		"Connection: %s\r\n",
		result.statusCode(), result.status().c_str(),
		date,
		shouldKeepAlive(result) ? KEEP_ALIVE : CLOSE);
}

void HttpResponse::send(const HttpResult &result)
{
	sendGenericHeader(mConnection, result);
	sendHeaders(mConnection, result);
	sendBody(mConnection, result);
}


HttpResponse::HttpResponse(struct mg_connection *conn)
	: mConnection(conn)
{}
