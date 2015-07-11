#include "Precompiled.h"

#include "WebServer.h"
#include <civetweb/include/CivetServer.h>
#include "HTTP/HttpArguments.h"
#include "HTTP/HttpRequest.h"
#include "HTTP/HttpResponse.h"
#include "WebServices/PatternURI.h"
#include "WebServices/WebService.h"
#include "HTTP/HttpErrors.h"
#include "HTTP/HttpResult.h"
#include "DTO/ErrorDTO.h"
#include "boost/uuid/random_generator.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include "HTTP/HttpDTOResult.h"
#include "HTTP/HttpMediaType.h"

namespace restless {

	const char *PORT = "8081";
	const char * options[] = { "listening_ports", PORT, 0 };


	class WebServer::WebServerImp : public CivetServer, public CivetHandler
	{
	private:
		struct CivetCallbacks : mg_callbacks
		{
			CivetCallbacks()
			{
				begin_request = WebServer::WebServerImp::begin_request;
				end_request = WebServer::WebServerImp::end_request;
				http_error = WebServer::WebServerImp::http_error;
				log_access = WebServer::WebServerImp::log_access;
				log_message = WebServer::WebServerImp::log_message;
			}
		};
		static CivetCallbacks sCivetCallbacks;

	private:
		WebServer& _intrf;

	private:
		virtual bool handleGet(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handlePost(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handlePut(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handleDelete(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handleOptions(CivetServer *server, struct mg_connection *conn) override;

	public:
		WebServerImp(WebServer *intrf)
			: CivetServer(options, &sCivetCallbacks)
			, _intrf(*intrf)
		{
			addHandler("/", this);
		}

		WebServerImp::~WebServerImp()
		{}

		static int begin_request(mg_connection *connection)
		{
			mg_context *ctx = mg_get_context(connection);
			WebServerImp *me = (WebServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onBeginRequest(connection);
		}

		static void end_request(const mg_connection *connection, int statusCode)
		{
			mg_context *ctx = mg_get_context(connection);
			WebServerImp *me = (WebServerImp *)mg_get_user_data(ctx);
			me->_intrf.onFinishRequest(connection, statusCode);
		}

		static int http_error(mg_connection *connection, int statusCode)
		{
			ErrorDTO errorDTO;
			errorDTO.id = boost::uuids::random_generator()();
			errorDTO.message = HttpStatus::fromInt(statusCode).defaultMessage();
			errorDTO.time = boost::posix_time::ptime();
			HttpResult *result = HttpDTOResult::createFromDTO(errorDTO, HttpMediaType::APPLICATION_JSON);
			HttpResponse response(connection);

			mg_context *ctx = mg_get_context(connection);
			WebServerImp *me = (WebServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onError(connection, statusCode);
		}

		static int log_access(const mg_connection *connection, const char *message)
		{
			mg_context *ctx = mg_get_context(connection);
			WebServerImp *me = (WebServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onLogAccess(connection, message);
		}

		static int log_message(const mg_connection *connection, const char *message)
		{
			mg_context *ctx = mg_get_context(connection);
			WebServerImp *me = (WebServerImp*)mg_get_user_data(ctx);
			return me->_intrf.onLogMessage(connection, message);
		}
	};


	bool WebServer::WebServerImp::handleOptions(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *req_info = mg_get_request_info(conn);
		HttpRequest request(conn, req_info);
		HttpResponse response(conn);
		HttpArguments queryArgs;
		queryArgs.set(request.query().params().begin(), request.query().params().end());

		for (auto delegateEntry : _intrf.mDelegates)
		{
			std::map<std::string, std::string> arguments;
			std::string remainingPath;
			if (PatternURI::Matches(delegateEntry.first, req_info->uri, &remainingPath, &arguments, true))
			{
				HttpArguments args(queryArgs);
				args.set(arguments.begin(), arguments.end());
				seed::AutoPtr<HttpResult> result;
				try
				{
					result = delegateEntry.second->handleOptions(request, remainingPath, args).givePtr();
				}
				catch (const std::exception &ex)
				{
					result = HttpErrors::createFromException(ex).givePtr();
				}

				if (result != nullptr)
				{
					response.send(*result);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool WebServer::WebServerImp::handleDelete(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *req_info = mg_get_request_info(conn);
		HttpRequest request(conn, req_info);
		HttpResponse response(conn);
		HttpArguments queryArgs;
		queryArgs.set(request.query().params().begin(), request.query().params().end());

		for (auto delegateEntry : _intrf.mDelegates)
		{
			std::map<std::string, std::string> arguments;
			std::string remainingPath;
			if (PatternURI::Matches(delegateEntry.first, req_info->uri, &remainingPath, &arguments, true))
			{
				HttpArguments args(queryArgs);
				args.set(arguments.begin(), arguments.end());
				seed::AutoPtr<HttpResult> result;
				try
				{
					result = delegateEntry.second->handleDelete(request, remainingPath, args).givePtr();
				}
				catch (const std::exception &ex)
				{
					result = HttpErrors::createFromException(ex).givePtr();
				}

				if (result != nullptr)
				{
					response.send(*result);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool WebServer::WebServerImp::handlePut(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *req_info = mg_get_request_info(conn);
		HttpRequest request(conn, req_info);
		HttpResponse response(conn);
		HttpArguments queryArgs;
		queryArgs.set(request.query().params().begin(), request.query().params().end());

		for (auto delegateEntry : _intrf.mDelegates)
		{
			std::map<std::string, std::string> arguments;
			std::string remainingPath;
			if (PatternURI::Matches(delegateEntry.first, req_info->uri, &remainingPath, &arguments, true))
			{
				HttpArguments args(queryArgs);
				args.set(arguments.begin(), arguments.end());
				seed::AutoPtr<HttpResult> result;
				try
				{
					result = delegateEntry.second->handlePut(request, remainingPath, args).givePtr();
				}
				catch (const std::exception &ex)
				{
					result = HttpErrors::createFromException(ex).givePtr();
				}
				catch (...)
				{
					result = HttpErrors::createFromException(HttpServerErrorException()).givePtr();
				}

				if (result != nullptr)
				{
					response.send(*result);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool WebServer::WebServerImp::handlePost(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *req_info = mg_get_request_info(conn);
		HttpRequest request(conn, req_info);
		HttpResponse response(conn);
		HttpArguments queryArgs;
		queryArgs.set(request.query().params().begin(), request.query().params().end());

		for (auto delegateEntry : _intrf.mDelegates)
		{
			std::map<std::string, std::string> arguments;
			std::string remainingPath;
			if (PatternURI::Matches(delegateEntry.first, req_info->uri, &remainingPath, &arguments, true))
			{
				HttpArguments args(queryArgs);
				args.set(arguments.begin(), arguments.end());
				seed::AutoPtr<HttpResult> result;
				try
				{
					result = delegateEntry.second->handlePost(request, remainingPath, args).givePtr();
				}
				catch (const std::exception &ex)
				{
					result = HttpErrors::createFromException(ex).givePtr();
				}

				if (result != nullptr)
				{
					response.send(*result);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool WebServer::WebServerImp::handleGet(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *req_info = mg_get_request_info(conn);
		HttpRequest request(conn, req_info);
		HttpResponse response(conn);
		HttpArguments queryArgs;
		queryArgs.set(request.query().params().begin(), request.query().params().end());

		for (auto &delegateEntry : _intrf.mDelegates)
		{
			std::map<std::string, std::string> arguments;
			std::string remainingPath;
			if (PatternURI::Matches(delegateEntry.first, req_info->uri, &remainingPath, &arguments, true))
			{
				HttpArguments args(queryArgs);
				args.set(arguments.begin(), arguments.end());
				seed::AutoPtr<HttpResult> result;
				try
				{
					result = delegateEntry.second->handleGet(request, remainingPath, args).givePtr();
				}
				catch (const std::exception &ex)
				{
					result = HttpErrors::createFromException(ex).givePtr();
				}

				if (result != nullptr)
				{
					response.send(*result);
					return true;
				}
			}
		}

		return _intrf.onError(conn, HttpStatus::eNOT_FOUND) != 1;
	}

	WebServer::WebServerImp::CivetCallbacks WebServer::WebServerImp::sCivetCallbacks;


	WebServer::WebServer(int port)
		: mImp(new WebServerImp(this))
	{}

	WebServer::~WebServer()
	{
		mImp.reset();
	}

} // namespace restless
