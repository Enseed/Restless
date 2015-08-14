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
		virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion) override
		{}

		virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override
		{}


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