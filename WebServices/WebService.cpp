#include "Precompiled.h"
#include "WebService.h"
#include "PatternURI.h"
#include "../HTTP/HttpArguments.h"
#include "../HTTP/HttpExceptions.h"
#include "../HTTP/HttpResult.h"
#include <civetweb/include/CivetServer.h>


namespace restless {

	class WebService::WebServiceImp
	{
	public:

	};

	seed::AutoPtr<HttpResult> WebService::handleGet(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments)
	{
		// if we have a sub delegate which can handle this call, forward it
		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleGet(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, true))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleGet(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		// if we don't handle this call, return the default (404)
		for (auto handler : mGetHandlers)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(handler.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				HttpResult *response = handler.second(request, args);
				if (response != nullptr)
					return response;
			}
		}

		return nullptr;
	}

	seed::AutoPtr<HttpResult> WebService::handlePost(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments)
	{
		// if we have a sub delegate which can handle this call, forward it
		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handlePost(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, true))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handlePost(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		// if we don't handle this call, return the default (404)
		for (auto handler : mPostHandlers)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(handler.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				HttpResult *response = handler.second(request, args);
				if (response != nullptr)
					return response;
			}
		}

		return nullptr;
	}

	seed::AutoPtr<HttpResult> WebService::handlePut(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments)
	{
		// if we have a sub delegate which can handle this call, forward it
		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handlePut(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, true))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handlePut(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		// if we don't handle this call, return the default (404)
		for (auto handler : mPutHandlers)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(handler.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				HttpResult *response = handler.second(request, args);
				if (response != nullptr)
					return response;
			}
		}

		return nullptr;
	}

	seed::AutoPtr<HttpResult> WebService::handleDelete(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments)
	{
		// if we have a sub delegate which can handle this call, forward it
		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleDelete(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, true))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleDelete(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		// if we don't handle this call, return the default (404)
		for (auto handler : mDeleteHandlers)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(handler.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				HttpResult *response = handler.second(request, args);
				if (response != nullptr)
					return response;
			}
		}

		return nullptr;
	}

	seed::AutoPtr<HttpResult> WebService::handleOptions(const HttpRequest &request, const std::string &subPath, const HttpArguments &arguments)
	{
		// if we have a sub delegate which can handle this call, forward it
		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, false))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleOptions(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		for (auto subDelegate : mDelegates)
		{
			std::map<std::string, std::string> moreArgs;
			std::string remainingPath;
			if (PatternURI::Matches(subDelegate.first, subPath, &remainingPath, &moreArgs, true))
			{
				HttpArguments args(arguments);
				args.set(moreArgs.begin(), moreArgs.end());
				seed::AutoPtr<HttpResult> response = subDelegate.second->handleOptions(request, remainingPath, args).givePtr();
				if (response != nullptr)
					return response.givePtr();
			}
		}

		// if we don't handle this call, return the default (404)
	// 	for (auto handler : mOptionsHandlers)
	// 	{
	// 		std::map<std::string, std::string> moreArgs;
	// 		std::string remainingPath;
	// 		if (PatternURI::Matches(handler.first, subPath, &remainingPath, &moreArgs, false))
	// 		{
	// 			HttpArguments args(arguments);
	// 			args.set(moreArgs.begin(), moreArgs.end());
	// 			HttpResult *response = handler.second(request, args);
	// 			if (response != nullptr)
	// 				return response;
	// 		}
	// 	}

		return nullptr;
	}

} // namespace restless