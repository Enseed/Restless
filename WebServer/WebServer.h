#ifndef __WRAPPER_WEBSERVER_H__
#define __WRAPPER_WEBSERVER_H__

#include "Enseed/Generic/AutoPtr/AutoPtr.h"
#include "../WebServices/WebDelegator.h"

struct mg_connection;

namespace restless {

	class WebServer : public WebDelegator
	{
	public:
		WebServer(int port);
		virtual ~WebServer();

		virtual int onBeginRequest(mg_connection *connection) { return 0; }
		virtual void onFinishRequest(const mg_connection *connection, int statusCode) { }
		virtual int onError(mg_connection *connection, int statusCode) { return 1; }
		virtual int onLogAccess(const mg_connection *connection, const char *message) { return 0; }
		virtual int onLogMessage(const mg_connection *connection, const char *message) { return 0; }

	public:
		class WebServerImp;

	private:
		seed::AutoPtr<WebServerImp> mImp;

	private:
		WebServer(WebServer &);
		WebServer& operator=(const WebServer&);
	};

} // namespace restless

#endif // __WRAPPER_WEBSERVER_H__
