#include "Precompiled.h"

#include "RestlessServer.h"
#include <civetweb/include/CivetServer.h>
#include "../HTTP/HttpArguments.h"
#include "../HTTP/HttpRequest.h"
#include "../HTTP/HttpResponse.h"
#include "../WebServices/PatternURI.h"
#include "../WebServices/WebService.h"
#include "../HTTP/HttpResult.h"

namespace restless {

	const char *PORT = "8081";
	const char * options[] = { "listening_ports", PORT, 0 };


	class RestlessServer::RestlessServerImp : public CivetServer, public CivetHandler
	{
	private:
		struct CivetCallbacks : mg_callbacks
		{
			CivetCallbacks()
			{
				begin_request = RestlessServer::RestlessServerImp::begin_request;
				end_request = RestlessServer::RestlessServerImp::end_request;
				http_error = RestlessServer::RestlessServerImp::http_error;
				log_access = RestlessServer::RestlessServerImp::log_access;
				log_message = RestlessServer::RestlessServerImp::log_message;
			}
		};
		static CivetCallbacks sCivetCallbacks;

	private:
		RestlessServer& _intrf;

	private:
		virtual bool handleGet(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handlePost(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handlePut(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handleDelete(CivetServer *server, struct mg_connection *conn) override;
		virtual bool handleOptions(CivetServer *server, struct mg_connection *conn) override;

	public:
		RestlessServerImp(RestlessServer *intrf)
			: CivetServer(options, &sCivetCallbacks)
			, _intrf(*intrf)
		{
			addHandler("/", this);
		}

		RestlessServerImp::~RestlessServerImp()
		{}

		static int begin_request(mg_connection *connection)
		{
			mg_context *ctx = mg_get_context(connection);
			RestlessServerImp *me = (RestlessServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onBeginRequest(connection);
		}

		static void end_request(const mg_connection *connection, int statusCode)
		{
			mg_context *ctx = mg_get_context(connection);
			RestlessServerImp *me = (RestlessServerImp *)mg_get_user_data(ctx);
			me->_intrf.onFinishRequest(connection, statusCode);
		}

		static int http_error(mg_connection *connection, int statusCode)
		{
			mg_context *ctx = mg_get_context(connection);
			RestlessServerImp *me = (RestlessServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onError(connection, statusCode);
		}

		static int log_access(const mg_connection *connection, const char *message)
		{
			mg_context *ctx = mg_get_context(connection);
			RestlessServerImp *me = (RestlessServerImp *)mg_get_user_data(ctx);
			return me->_intrf.onLogAccess(connection, message);
		}

		static int log_message(const mg_connection *connection, const char *message)
		{
			mg_context *ctx = mg_get_context(connection);
			RestlessServerImp *me = (RestlessServerImp*)mg_get_user_data(ctx);
			return me->_intrf.onLogMessage(connection, message);
		}
	};


	bool RestlessServer::RestlessServerImp::handleOptions(CivetServer *server, struct mg_connection *conn)
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

					if (result != nullptr)
					{
						response.send(*result);
						return true;
					}
				}
				catch (const std::exception &ex)
				{
					result = _intrf.onException(conn, ex).givePtr();

					if (result == nullptr)
						_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					else
						response.send(*result);

					return true;
				}
				catch (...)
				{
					_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool RestlessServer::RestlessServerImp::handleDelete(CivetServer *server, struct mg_connection *conn)
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

					if (result != nullptr)
					{
						response.send(*result);
						return true;
					}
				}
				catch (const std::exception &ex)
				{
					result = _intrf.onException(conn, ex).givePtr();

					if (result == nullptr)
						_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					else
						response.send(*result);

					return true;
				}
				catch (...)
				{
					_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool RestlessServer::RestlessServerImp::handlePut(CivetServer *server, struct mg_connection *conn)
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

					if (result != nullptr)
					{
						response.send(*result);
						return true;
					}
				}
				catch (const std::exception &ex)
				{
					result = _intrf.onException(conn, ex).givePtr();

					if (result == nullptr)
						_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					else
						response.send(*result);

					return true;
				}
				catch (...)
				{
					_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool RestlessServer::RestlessServerImp::handlePost(CivetServer *server, struct mg_connection *conn)
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

					if (result != nullptr)
					{
						response.send(*result);
						return true;
					}
				}
				catch (const std::exception &ex)
				{
					result = _intrf.onException(conn, ex).givePtr();

					if (result == nullptr)
						_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					else
						response.send(*result);

					return true;
				}
				catch (...)
				{
					_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					return true;
				}
			}
		}

		_intrf.onError(conn, HttpStatus::eNOT_FOUND);
		return true;
	}

	bool RestlessServer::RestlessServerImp::handleGet(CivetServer *server, struct mg_connection *conn)
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

					if (result != nullptr)
					{
						response.send(*result);
						return true;
					}
				}
				catch (const std::exception &ex)
				{
					result = _intrf.onException(conn, ex).givePtr();

					if (result == nullptr)
						_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					else
						response.send(*result);

					return true;
				}
				catch (...)
				{
					_intrf.onError(conn, HttpStatus::eSERVER_ERROR);
					return true;
				}
			}
		}

		return _intrf.onError(conn, HttpStatus::eNOT_FOUND) != 1;
	}

	RestlessServer::RestlessServerImp::CivetCallbacks RestlessServer::RestlessServerImp::sCivetCallbacks;


	RestlessServer::RestlessServer(int port)
		: mImp(new RestlessServerImp(this))
	{}

	RestlessServer::~RestlessServer()
	{
		mImp.reset();
	}

} // namespace restless
