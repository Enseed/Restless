#include "Precompiled.h"
#include "WebDelegator.h"
#include "WebService.h"


WebDelegator::~WebDelegator()
{
	mDelegates.clear();
}

WebServiceRef WebDelegator::delegateSubPath(const std::string &iPath, const WebServiceRef &iDelegate)
{
	mDelegates[iPath] = iDelegate;
	return iDelegate;
}
