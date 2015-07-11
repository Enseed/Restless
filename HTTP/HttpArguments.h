#pragma once
#include <map>
#include <string>
#include "HttpExceptions.h"

class HttpArguments
{
private:
	std::map<std::string, std::string> mArguments;

public:
	HttpArguments(const HttpArguments &rhs)
		: mArguments(rhs.mArguments)
	{}

	HttpArguments()
	{}

	const std::map<std::string, std::string> arguments() const
	{
		return mArguments;
	}

	template<class T>
	T get(const std::string &key) const
	{
		auto found = mArguments.find(key);
		if (found != mArguments.end())
			return boost::lexical_cast<T>(found->second);
		else
			return T();
	}

	std::string get(const std::string &key) const
	{
		auto found = mArguments.find(key);
		if (found != mArguments.end())
			return found->second;
		else
			return std::string();
	}

	template<class T>
	T require(const std::string &key) const
	{
		auto found = mArguments.find(key);
		if (found != mArguments.end())
		{
			try
			{
				return boost::lexical_cast<T>(found->second);
			}
			catch (const std::exception &/*ex*/)
			{
				std::string value = boost::lexical_cast<std::string>(found->second);
				throw HttpClientErrorInvalidArgumentException<T>(key, value);
			}
		}

		throw HttpClientErrorMissingArgumentException(key);
	}

	template<class T>
	T requireRange(const std::string &key, const T &valStart, const T &valEnd) const
	{
		T val = require<T>(key);
		if (val >= valStart && val < valEnd)
		{
			return val;
		}
		else
		{
			throw HttpClientErrorOutOfRangeArgumentException<T>(key, require<std::string>(key), valStart, valEnd);
		}
	}

	std::string require(const std::string &key) const
	{
		std::string value = get(key);
		if (!value.empty())
			return value;

		throw HttpClientErrorMissingArgumentException(key);
	}

	void set(const std::string &key, const std::string &value)
	{
		mArguments[key] = value;
	}

	template<class TITER>
	void set(const TITER &b, const TITER &e)
	{
		mArguments.insert(b, e);
	}
};