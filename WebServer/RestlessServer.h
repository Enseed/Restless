#ifndef __WRAPPER_RESTLESSSERVER_H__
#define __WRAPPER_RESTLESSSERVER_H__
	
#include "Enseed/Generic/AutoPtr/AutoPtr.h"
#include "../WebServices/WebDelegator.h"
#include "../HTTP/HttpResult.h"
#include <exception>

struct mg_connection;

namespace restless {

	class RestlessServer : public WebDelegator
	{
	public:
		RestlessServer(int port);
		virtual ~RestlessServer();

		virtual int onBeginRequest(mg_connection *connection) { return 0; }
		virtual void onFinishRequest(const mg_connection *connection, int statusCode) { }
		virtual int onError(mg_connection *connection, int statusCode) { return 1; }
		virtual int onLogAccess(const mg_connection *connection, const char *message) { return 0; }
		virtual int onLogMessage(const mg_connection *connection, const char *message) { return 0; }

		virtual sd::AutoPtr<HttpResult> onException(mg_connection *connection, const std::exception &exception) { return nullptr; }

	public:
		class RestlessServerImp;

	private:
		seed::AutoPtr<RestlessServerImp> mImp;

	private:
		RestlessServer(RestlessServer &);
		RestlessServer& operator=(const RestlessServer&);
	};

} // namespace restless

#endif // __WRAPPER_RESTLESSSERVER_H__
