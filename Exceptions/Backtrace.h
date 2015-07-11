#pragma once
#include <CodeProject/Stackwalker/StackWalker.h>
#include <string>

class Stacktrace
{
	class MyStackWalker : public StackWalker
	{
	private:
		MyStackWalker(const MyStackWalker&);
		MyStackWalker& operator=(const MyStackWalker&);

	public:
		std::stringstream mStackStringStream;

	public:
		MyStackWalker()
		{
			ShowCallstack();
		}

	protected:
		virtual void OnOutput(LPCSTR szText)
		{
			mStackStringStream << szText;
		}
	};

private:
	Stacktrace(const Stacktrace &rhs);
	Stacktrace& operator=(const Stacktrace&);

public:
	Stacktrace()
	{}

public:
	std::string toString() const
	{
		return mSW.mStackStringStream.str();
	}

private:
	MyStackWalker mSW;
};