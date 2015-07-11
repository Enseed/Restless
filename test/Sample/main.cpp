#include "Enseed/Restless/WebServer/WebServer.h"
#include "Enseed/Restless/WebServices/WebService.h"

class HelloWorldWebService : public restless::WebService
{

};


class WebServer : public restless::WebServer
{
public:
	WebServer()
		: restless::WebServer(8081)
	{
		delegateSubPath("/hello", WebServiceRef(new HelloWorldWebService));
	}
};

int main(int argc, const char *argv[])
{
	WebServer webServer;
	Sleep(1000000);
	return 0;
}