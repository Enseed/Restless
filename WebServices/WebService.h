#pragma once

#include "WebDelegator.h"
#include "boost/function.hpp"
#include "Generic/AutoPtr/AutoPtr.h"

class HttpRequest;
class HttpArguments;
class HttpResult;

namespace restless {

	class WebService : public WebDelegator
	{
	public:
		typedef boost::function<HttpResult* (const HttpRequest&, const HttpArguments&)> MethodHandler;

	private:
		std::map<std::string, MethodHandler> mGetHandlers;
		std::map<std::string, MethodHandler> mPutHandlers;
		std::map<std::string, MethodHandler> mPostHandlers;
		std::map<std::string, MethodHandler> mDeleteHandlers;

	public:
		void onGet(const std::string &subpath, const MethodHandler &aHandler)
		{
			mGetHandlers[subpath] = aHandler;
		}

		void onGet(const MethodHandler &aHandler)
		{
			onGet(std::string(), aHandler);
		}

		void onPut(const std::string &subpath, const MethodHandler &aHandler)
		{
			mPutHandlers[subpath] = aHandler;
		}

		void onPut(const MethodHandler &aHandler)
		{
			onPut(std::string(), aHandler);
		}

		void onPost(const std::string &subpath, const MethodHandler &aHandler)
		{
			mPostHandlers[subpath] = aHandler;
		}

		void onPost(const MethodHandler &aHandler)
		{
			onPost(std::string(), aHandler);
		}

		void onDelete(const std::string &subpath, const MethodHandler &aHandler)
		{
			mDeleteHandlers[subpath] = aHandler;
		}

		void onDelete(const MethodHandler &aHandler)
		{
			onDelete(std::string(), aHandler);
		}

	private:
		class WebServiceImp;
		sd::AutoPtr<WebService> _imp;

	public:
		virtual seed::AutoPtr<HttpResult> handleGet(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments);
		virtual seed::AutoPtr<HttpResult> handlePost(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments);
		virtual seed::AutoPtr<HttpResult> handlePut(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments);
		virtual seed::AutoPtr<HttpResult> handleDelete(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments);
		virtual seed::AutoPtr<HttpResult> handleOptions(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments);
	};

} // namespace restless