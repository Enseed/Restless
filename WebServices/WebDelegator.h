#pragma once

#include <Generic/CountPtr/CountPtr.h>
#include <map>

namespace restless { class WebService; }

typedef sd::CountPtr<restless::WebService> WebServiceRef;

class WebDelegator
{
protected:
	std::map<std::string, WebServiceRef> mDelegates;

public:
	WebDelegator()
		: mDelegates()
	{}

	virtual ~WebDelegator();

	WebServiceRef delegateSubPath(const std::string &iPath, const WebServiceRef &iDelegate);

private:
	WebDelegator(const WebDelegator&);
	WebDelegator& operator=(const WebDelegator&);
};


